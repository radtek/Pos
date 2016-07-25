unit rAnalysis;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls,IBTable;

type
  TfrmAnalysis = class(TForm)
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
    QRLabel11: TQRLabel;
    QRLabel12: TQRLabel;
    QRLabel13: TQRLabel;
    QRLabel14: TQRLabel;
    QRLabel15: TQRLabel;
    QRLabel16: TQRLabel;
    QRLabel17: TQRLabel;
    QRLabel18: TQRLabel;
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
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    RichEdit: TQRRichText;
    QRLabel2: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel5: TQRLabel;
    QRDBText8: TQRDBText;
    QRDBText9: TQRDBText;
    QRLabel7: TQRLabel;
    QRLabel19: TQRLabel;
    QRExpr8: TQRExpr;
    QRExpr7: TQRExpr;
    procedure qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
    function Extract ( qrPrint : Boolean ; PrtIndex : SmallInt ;tblName : string ): Boolean;
    procedure QRBand4BeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmAnalysis: TfrmAnalysis;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
var
    Printing : Boolean;
{ **************************************************************************** }
procedure TfrmAnalysis.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ **************************************************************************** }
function TfrmAnalysis.Extract ( qrPrint : Boolean; PrtIndex : SmallInt ;tblName : string ): Boolean;
var Totctr,MNUM,FNUM,NoNUM,NullCtr : Integer;
    tblTemp : TIBTable;
    value : extended;
const SQL1 = 'SELECT COUNT(*) FROM MEMBERS m , M_GROUPS mg where m.MEMBER = mg.MEMBER and m.SUB_PAID > 0';
//const SQL1 = 'SELECT COUNT(*) FROM MEMBERS where SUB_PAID > 0';
//const SQL1 = 'SELECT COUNT(*) FROM MEMBERS where MALE_FEMALE in (%s,%s)';
      SQL2 = 'SELECT COUNT(*) FROM MEMBERS m, M_GROUPS mg WHERE mg.GROUPS = %d ' +
             'and m.SUB_PAID > 0 and m.MEMBER = mg.MEMBER and m.MALE_FEMALE %s';
      SQL3 = 'select m.member,mg.GROUPS,m.MALE_FEMALE,m.SUB_PAID from MEMBERS M LEFT JOIN M_GROUPS MG ' +
              'on m.MEMBER = MG.MEMBER';
             
begin
     result := False;
     TotCtr := 0;FNUM := 0;MNUM := 0;NoNUM := 0;
     try
        with dm.qryGeneral do begin
           dm.qryReport.Close;
           dm.qryReport.SQL.Text := Format('CREATE TABLE %s ( DESCRIPTION CHAR(30),' +
              'MNUM Float,MPER Float,FNUM Float,FPER Float,NoNUM Float,NoPER Float,TOTNUM Float,TOTPER Float)',[tblName]);
           dm.qryReport.open;
           dm.IBCommitRetain;
           tblTemp := TIBTable.create(nil);
           tblTemp.Database := dm.IBDatabase;
           tblTemp.Transaction := dm.IBTransaction;
           tblTemp.Tablename := tblName;
           tblTemp.Open;
           Close;
           // gets Count of Members
           SQL.Text := SQL1;
           execquery;
           if not EOF then TotCtr :=  Fields[0].AsInteger;
           Close;
           // gets No Groups
           SQL.Text := SQL3;
           execquery;
           if not EOF then begin
              NullCtr := 0;
              while not EOF do begin
                 // no Group and Financial
                 if (FieldByName('GROUPS').IsNull) and (FieldByName('SUB_PAID').AsInteger > 0) then begin
                    inc(NullCtr);
                    if FieldByName('MALE_FEMALE').AsString = 'M' then begin
                       inc(MNUM);
                    end else if FieldByName('MALE_FEMALE').AsString = 'F' then begin
                       inc(FNUM);
                    end else begin
                       inc(NoNUM);
                    end;    
                 end;
                 Next;
              end;   
              TotCtr := TotCtr + NullCtr;
              tblTemp.insert;
              tblTemp.FieldByName('DESCRIPTION').AsString := '**** NO Group ****';
              tblTemp.FieldByName('FNUM').AsInteger := FNUM;
              tblTemp.FieldByName('MNUM').AsInteger := MNUM;
              tblTemp.FieldByName('NoNUM').AsInteger := NoNUM;
              tblTemp.FieldByName('TOTNUM').AsInteger := MNUM + FNUM + NoNUM;
              
              if MNUM <> 0.00 then begin
                 value := (MNUM / TotCtr) * 100;
                 //tblTemp.FieldByName('MPER').AsFloat := Round((Value * 1000) ) / 1000.0;
                 tblTemp.FieldByName('MPER').AsFloat := Value;
              end else begin
                 tblTemp.FieldByName('MPER').AsFloat := 0.00;
              end;   
              if FNUM <> 0.00 then begin
                 value := (FNUM / TotCtr) * 100;
                 tblTemp.FieldByName('FPER').AsFloat := Value;
                 //tblTemp.FieldByName('FPER').AsFloat := Round((Value * 1000) ) / 1000.0;
              end else begin
                 tblTemp.FieldByName('FPER').AsFloat := 0.00;
              end;   
              if NoNUM <> 0.00 then begin
                 value := (NoNUM / TotCtr) * 100;
                 tblTemp.FieldByName('NoPER').AsFloat := Value;
                 //tblTemp.FieldByName('NoPER').AsFloat := Round((Value * 1000) ) / 1000.0;
              end else begin
                 tblTemp.FieldByName('NoPER').AsFloat := 0.00;
              end;
              tblTemp.FieldByName('TOTPER').AsFloat := tblTemp.FieldByName('MPER').AsFloat +
                                                       tblTemp.FieldByName('FPER').AsFloat +
                                                       tblTemp.FieldByName('NoPER').AsFloat;
              tblTemp.post;
           end;
           FNUM := 0;MNUM := 0;NoNUM := 0;
           dm.tblGroups.Open;
           dm.tblGroups.First;
           while not dm.tblGroups.EOF do begin  // loop thru groups and get numbers by male /female
              Close;
              SQL.Text := format(SQL2,[dm.tblGroups.FieldByName('GROUPS').AsInteger,'= ''M''']);
              execquery;
              if not EOF then MNUM :=Fields[0].AsInteger;
              Close;
              SQL.Text := format(SQL2,[dm.tblGroups.FieldByName('GROUPS').AsInteger,'= ''F''']);
              execquery;
              if not EOF then FNUM :=  Fields[0].AsInteger;
              Close;
              SQL.Text := format(SQL2,[dm.tblGroups.FieldByName('GROUPS').AsInteger,'is NULL']);
              execquery;
              if not EOF then NoNUM :=  Fields[0].AsInteger;
              tblTemp.insert;
              tblTemp.FieldByName('DESCRIPTION').AsString := dm.tblGroups.FieldByName('DESCRIPTION').AsString;
              tblTemp.FieldByName('FNUM').AsInteger := FNUM;
              tblTemp.FieldByName('MNUM').AsInteger := MNUM;
              tblTemp.FieldByName('NoNUM').AsInteger := NoNUM;
              tblTemp.FieldByName('TOTNUM').AsInteger := MNUM + FNUM + NoNUM;
              
              if MNUM <> 0.00 then begin
                 value := (MNUM / TotCtr) * 100;
                 //tblTemp.FieldByName('MPER').AsFloat := Round((Value * 1000) ) / 1000.0;
                 tblTemp.FieldByName('MPER').AsFloat := Value;
              end else begin
                 tblTemp.FieldByName('MPER').AsFloat := 0.00;
              end;   
              if FNUM <> 0.00 then begin
                 value := (FNUM / TotCtr) * 100;
                 tblTemp.FieldByName('FPER').AsFloat := Value;
                 //tblTemp.FieldByName('FPER').AsFloat := Round((Value * 1000) ) / 1000.0;
              end else begin
                 tblTemp.FieldByName('FPER').AsFloat := 0.00;
              end;   
              if NoNUM <> 0.00 then begin
                 value := (NoNUM / TotCtr) * 100;
                 tblTemp.FieldByName('NoPER').AsFloat := Value;
                 //tblTemp.FieldByName('NoPER').AsFloat := Round((Value * 1000) ) / 1000.0;
              end else begin
                 tblTemp.FieldByName('NoPER').AsFloat := 0.00;
              end;
              tblTemp.FieldByName('TOTPER').AsFloat := tblTemp.FieldByName('MPER').AsFloat +
                                                       tblTemp.FieldByName('FPER').AsFloat +
                                                       tblTemp.FieldByName('NoPER').AsFloat;
              tblTemp.post;
              dm.tblGroups.Next;
           end;
        end;
        //dm.IBCommitRetain;
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        Printing := qrPrint;
        with dm.qryReport do begin
           Close;
           SQL.Text := format('SELECT * FROM %s',[tblName]);
           Open;
        end;
        result := True;
     finally
        dm.tblGroups.Close;
        tblTemp.close;
        tblTemp.free;
     end;
end;
{ **************************************************************************** }
procedure TfrmAnalysis.QRBand4BeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
const SQL1 = 'select sub_paid, count(sub_paid) as Ctr from members group by sub_paid';
      SQL2 = 'select is_active, count(is_active) as Ctr from members group by is_active';
      SQL3 = 'select member,is_active from members where is_active is null';
      tab = chr(09);
      TotMemMsg = 'Total Members';
      ActiveMsg = 'Active';
var SubPayTypes : TSubPayTypes;
      i : smallint;
begin
     with dm.qryGeneral do begin
        RichEdit.Lines.Add( TotMemMsg  + tab + GetMemberCount);
        RichEdit.Lines.Add('');
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL1) then begin
           while not EOF do begin
              SubPayTypes := TSubPayTypes(FieldByName('SUB_PAID').AsInteger);
              RichEdit.Lines.Add(FinancialArray[ord(SubPayTypes)] + tab + FieldByName('Ctr').AsString);
              next;
           end;
        end;
        RichEdit.Lines.Add('');
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL2) then begin
           while not EOF do begin
              if FieldByName('IS_ACTIVE').AsString = varNo then
                 RichEdit.Lines.Add( 'Non ' + ActiveMsg + tab + FieldByName('Ctr').AsString);
              if FieldByName('IS_ACTIVE').AsString = varYes then
                 RichEdit.Lines.Add( ActiveMsg + tab + tab + FieldByName('Ctr').AsString);
              next;
           end;
        end;
        // cant count up the nulls
        if dm.sqlExecQueryCheckEOF(dm.qryGeneral,SQL3) then begin
           i := 0;
           while not EOF do begin
              inc(i);
              next;
           end;
           RichEdit.Lines.Add( 'No Entry' + tab + intToStr(i));
        end;
        dm.qryGeneral.Close;
     end;
end;
{ **************************************************************************** }
end.
