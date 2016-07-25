unit rCurrentPoints;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, QuickRpt, Qrctrls;

type
  TfrmCurrentPoints = class(TForm)
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
    qrFooterBand: TQRBand;
    qrGroupBand: TQRGroup;
    dbtGroups: TQRDBText;
    QRDBText3: TQRDBText;
    QRExpr1: TQRExpr;
    QRExpr4: TQRExpr;
    QRExpr5: TQRExpr;
    lblGroups: TQRLabel;
    QRLabel1: TQRLabel;
    lblFinancial: TQRLabel;
    QRLabel2: TQRLabel;
    QRLabel3: TQRLabel;
    procedure QuickRepBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(qrprint : Boolean ;PrtIndex,Lines : shortint ; mGroups,SortBy : shortstring) : Boolean;
    function ExtractSection(qrprint : Boolean; PrtIndex,Lines : shortint ; mSections : shortstring) : Boolean;
    procedure qrDetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    function ExtractTop50(qrprint : Boolean ;PrtIndex : shortint; mGroups : ShortString) : Boolean;
  private
    { Private declarations }
    FTop50 : Integer;
    FExtractTop50 : Boolean;
  public
    { Public declarations }
  end;

var
  frmCurrentPoints: TfrmCurrentPoints;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
var
    Printing : Boolean;

{ **************************************************************************** }
procedure TfrmCurrentPoints.QuickRepBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
    FTop50 := 0;
end;
{ **************************************************************************** }
function TfrmCurrentPoints.Extract(qrprint : Boolean ;PrtIndex,Lines : shortint ; mGroups,SortBy : ShortString) : Boolean;
const  SQL1 = 'SELECT m.MEMBER,m.EXPIRES,m.FULL_NAME,m.KNOWN_AS,m.POINTS_AVAILABLE,m.TOTAL_REDEEMED,m.TOTAL_EARNT,' +
              'm.BONUS_CREDIT + m.EARNT_CREDIT + m.PRE_PAID_CREDIT as VALUE_OF_CREDIT,m.BONUS_CREDIT,m.EARNT_CREDIT,g.GROUPS,g.DESCRIPTION,m.SUB_PAID FROM MEMBERS m, ' +
              'GROUPS g,M_GROUPS mg where m.MEMBER = mg.MEMBER and mg.GROUPS = g.GROUPS ' +
              'and POINTS_AVAILABLE <> 0';
       SQL2 = 'SELECT g.DESCRIPTION,g.GROUPS,m.SUB_PAID,0 AS MEMBER,NULL AS FULL_NAME,SUM(m.POINTS_AVAILABLE) as POINTS_AVAILABLE,' +
              'SUM(m.TOTAL_REDEEMED) AS TOTAL_REDEEMED,SUM(m.TOTAL_EARNT) AS TOTAL_EARNT,' +
              'SUM(m.TOTAL_REDEEMED) + SUM(m.BONUS_CREDIT) +  SUM(m.EARNT_CREDIT) AS VALUE_OF_CREDIT,SUM(m.BONUS_CREDIT) AS BONUS_CREDIT,' +
              'SUM(m.EARNT_CREDIT) AS EARNT_CREDIT,m.SUB_PAID  FROM MEMBERS m, GROUPS g, ' +
              'M_GROUPS mg where m.MEMBER = mg.MEMBER and mg.GROUPS = g.GROUPS ';
              
begin
     result := False;
     FExtractTop50 := FAlse;
     case lines of
     0 : begin // detail
            with dm.qryReport do begin
                close;
                SQL.Text := SQL1;
                if mGroups <> '' then SQL.Add( Format(' and mg.GROUPS in (%s)',[mGroups]));
                // 
                system.delete(SortBy,1,pos('m.',SortBy) -1);
                SQL.Add(format(' ORDER BY g.GROUPS,%s',[SortBy]));
                open;
            end;
         end;
     1 : begin // summary only;
            //qrGroupBand.Enabled    := False;
            //qrFooterBand.Enabled   := False;
            dbtMember.DataField    := 'GROUPS';
            dbtFullName.DataField  := 'DESCRIPTION';
            with dm.qryReport do begin
                close;
                SQL.Text := SQL2;
                if mGroups <> '' then SQL.Add( Format(' and mg.GROUPS in (%s)',[mGroups]));
                SQL.Add( ' GROUP by g.DESCRIPTION,g.GROUPS,m.SUB_PAID order by g.GROUPS' );
                addtoclipboard(SQL.text);
                open;
            end;
         end;
     end; //case
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     Printing := qrPrint;
     result := True;
end;
{ **************************************************************************** }
function TfrmCurrentPoints.ExtractTop50(qrprint : Boolean ;PrtIndex : shortint; mGroups : ShortString) : Boolean;
const SQL1 = 'SELECT MEMBER,FULL_NAME,m.EXPIRES,m.KNOWN_AS,SUB_PAID,POINTS_AVAILABLE,TOTAL_REDEEMED,TOTAL_EARNT ' +
             'FROM MEMBERS m , M_GROUPS mg where m.MEMBER = mg.MEMBER %s order by TOTAL_EARNT desc';
begin                    
     result := False;
     FExtractTop50 := True;
     qrGroupBand.Enabled := False;
     with dm.qryReport do begin
         close;
         if mGroups <> '' then
             SQL.Text := format(SQL1,['and mg.GROUPS in (' + mGroups + ')'])
         else
            SQL.Text := format(SQL1,['']);  // blank out %s
         open;
     end; //case
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     Printing := qrPrint;
     result := True;
end;
{ **************************************************************************** }
function TfrmCurrentPoints.ExtractSection(qrprint : Boolean ;PrtIndex,Lines : shortint ; mSections : ShortString) : Boolean;
begin
     result := False;
     FExtractTop50 := FAlse;
     lblGroups.Caption := 'Section';
     dbtGroups.DataField := 'SECTION';
     case lines of
     0 : begin // detail
            with dm.qryReport do begin
                close;
                SQL.Text := 'SELECT M.MEMBER,M.FULL_NAME,m.EXPIRES,m.KNOWN_AS,m.POINTS_AVAILABLE,M.TOTAL_REDEEMED,M.TOTAL_EARNT,m.SUB_PAID,' +
                            'M.BONUS_CREDIT + M.EARNT_CREDIT + PRE_PAID_CREDIT as VALUE_OF_CREDIT,M.BONUS_CREDIT,M.EARNT_CREDIT, MS.SECTION,S.DESCRIPTION ' +
                            'FROM MEMBERS M, M_SECTIONS MS, SECTIONS S  ' +
                            'WHERE M.MEMBER = MS.MEMBER and MS.SECTION = S.SECTION';
                if mSections <> '' then SQL.Add( Format(' AND MS.SECTION in (%s)',[mSections]));
                SQL.Add(' ORDER BY MEMBER');
                //sm(SQL.TExt);
                open;
            end;
         end;
     1 : begin // summary only
            with dm.qryReport do begin
                qrGroupBand.Enabled    := False;
                qrFooterBand.Enabled   := False;
                dbtMember.DataField    := 'SECTION';
                dbtFullName.DataField  := 'DESCRIPTION';
                close;
                SQL.Text := 'SELECT 0 as MEMBER,null as SUB_PAID,NULL AS FULL_NAME,SUM(M.POINTS_AVAILABLE) as POINTS_AVAILABLE,' +
                            'SUM(M.TOTAL_REDEEMED) AS TOTAL_REDEEMED,SUM(M.TOTAL_EARNT) AS TOTAL_EARNT,' +
                            'SUM(M.BONUS_CREDIT) + SUM(M.EARNT_CREDIT) + SUM(M.PRE_PAID_CREDIT) AS VALUE_OF_CREDIT,SUM(M.BONUS_CREDIT) AS BONUS_CREDIT,' +
                            'SUM(M.EARNT_CREDIT) AS EARNT_CREDIT, S.SECTION,S.DESCRIPTION ' + 
                            'FROM MEMBERS M, M_SECTIONS MS, SECTIONS S '+
                            'WHERE M.MEMBER = MS.MEMBER and MS.SECTION = S.SECTION';
                if mSections <> '' then SQL.Add( Format(' AND S.SECTION in (%s)',[mSections]));
                SQL.Add( ' GROUP by S.DESCRIPTION,S.SECTION order by S.SECTION' );
                open;
            end;
         end;
     end; //case
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     Printing := qrPrint;
     result := True;
end;

{ **************************************************************************** }
procedure TfrmCurrentPoints.qrDetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     if FExtractTop50 and (FTop50 >= 50) then PrintBand := False;
     inc(FTop50);
     with dm.qryReport do begin
        if not FieldByName('SUB_PAID').IsNull then
           lblFinancial.Caption := YesNoTypesArray[FieldByName('SUB_PAID').AsInteger];
     end;
end;
{ **************************************************************************** }

end.
