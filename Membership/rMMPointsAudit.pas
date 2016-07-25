unit rMMPointsAudit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls,IBTable,IBQuery,  Db,
  IBCustomDataSet ;

type
  TfrmPointsHistory = class(TForm)
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
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    QRLabel7: TQRLabel;
    qrDetailBand: TQRBand;
    QRDBText1: TQRDBText;
    QRDBText4: TQRDBText;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText6: TQRDBText;
    qrSummaryBand: TQRBand;
    QRExpr3: TQRExpr;
    QRExpr7: TQRExpr;
    QRExpr6: TQRExpr;
    QRExpr8: TQRExpr;
    QRExpr9: TQRExpr;
    QRExpr10: TQRExpr;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRLabel2: TQRLabel;
    QRDBText7: TQRDBText;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(PrtIndex : shortint; DateFrom,DateTo,MMDateTimeFrom,MMDateTimeTo : TDAteTime; tblName : string  ) : Boolean;
    procedure LoadPointsAudit(var tblTemp : TIBTable);
    procedure LoadPointsAuditWithDates(DateFrom,DateTo : TDateTime);
    
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPointsHistory: TfrmPointsHistory;

implementation

{$R *.DFM}
uses Global,
     dmMembers,
     dmMenuMate;
{ ****************************************************************************}     
procedure TfrmPointsHistory.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmPointsHistory.Extract(PrtIndex : shortint; DateFrom,DateTo,MMDateTimeFrom,MMDateTimeTo : TDAteTime ;tblName : string   ) : Boolean;
        
var  tblTemp : TIBTable;
     MemDateFrom,MemDateTo: TDateTime;
     i,intDateFrom,intDateTo : Integer;
     v : Variant;
     
const
      MMsql =
		'Select A.Item_Name, cast(A.Time_Stamp_Billed as Date) TIME_STAMP,Sum(A.Points_Earned) Points_Earned,Sum(A.Redeemed) Points_Redeemed  ' +
		'From Archive A ' + // Inner Join Security S On A.Security_Ref = S.Security_Ref ' +
		'Where A.Time_Stamp_Billed between :DateFrom And :DateTo ' + //And S.Security_Event = ''Ordered By'' ' +
		'Group By A.Item_Name,A.Time_Stamp_Billed Union All ' +
		'Select D.Item_Name, cast(A.Time_Stamp_Billed as Date) TIME_STAMP,Sum(D.Points_Earned) Points_Earned,Sum(D.Redeemed) Points_Redeemed  ' +
		'From DayArchive D ' + //Inner Join Security S On D.Security_Ref = S.Security_Ref ' +
		'Where D.Time_Stamp_Billed between :DateFrom And :DateTo ' + //And S.Security_Event = ''Ordered By'' ' +
		'Group By D.Item_Name,A.Time_Stamp_Billed Order by 1,2 ';
      SQL1 = 'Update %s set PROCESS = -1 where ITEM_NAME = ''%s''';

      SQL2 = 'insert into %s select ''POINTS_LOG'',cast (EDIT_DATE  as Date),%s,0,%d ' +
             'from POINTS_LOG where PROCESS in ( %s ) and EDIT_DATE between ''%s'' and ''%s''';

      SQL3 = 'Select TIME_STAMP,PROCESS, sum(POINTS_EARNT) as POINTS_EARNT,sum(POINTS_SPENT) as POINTS_SPENT ' +
             'from %s group by TIME_STAMP,PROCESS order by TIME_STAMP,PROCESS'; 
      SQL4 = 'CREATE TABLE %s ( ITEM_NAME varCHAR(50),TIME_STAMP Date,POINTS_EARNT Float,POINTS_SPENT Float,PROCESS Integer)';
{     
 Location is used for the Item_Name
 Bonus_Points is used to store :-
 -2 for Sale
 -1 for VIP Credits
 2 for Adjustments ( in Maintenance and Settings )
 3 for Subs ( pos and Office )
 10 for Deleted ( Deleted members, Delete Non Financial members ( Rollover ) and Reset of Points ) 
}             
begin                    
     result := False;
     try
        // clear table
        dm.sqlExecQuery(dm.qryGeneral,'Delete from POINTS_AUDIT');
        dm.IBCommitRetain;
        // create table
        dm.qryOpen(dm.qryReport ,format(SQL4,[tblName]));
        dm.qryReport.Close;
        dm.IBCommitRetain;
        // create temp table
        tblTemp := TIBTable.create(nil); 
        tblTemp.Database    := dm.IBDatabase;
        tblTemp.Transaction := dm.IBTransaction;
        tblTemp.Tablename   := tblName;
        tblTemp.Open;
        // get data from MenuMate
        dmMM.qryMMReport.close;
        //MMDateFrom  := DateFrom + 0.21;  // 5 oclock that day - same as the MenuMate reports
        //MMDateTo    := DateTo + 1.21;    // 5 oclock on the next day
        MemDateFrom := DateFrom;         // 
        MemDateTo   := DateTo + 0.999999;     // add time to midnight
        //showmessage(DateTimeToStr(MemDateFrom) + ' ' + DateTimeToStr(MemDateTo));
        dmMM.qryMMReport.SQL.Text := mmSQL;
        dmMM.qryMMReport.ParamByName('DateFrom').AsDateTime := MMDateTimeFrom;
        dmMM.qryMMReport.ParamByName('DateTo').AsDateTime   := MMDateTimeTo;
        dmMM.qryMMReport.open;
        LoadPointsAudit(tblTemp);
        dmMM.qryMMReport.Close;
        dmMM.MMTransaction.Commit;
        // VIP Credit Name
        if Global.VIPCreditName <> '' then 
          dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[tblName,Global.VIPCreditName]));
        // adjustments
        dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[tblName,'POINTS_AVAILABLE - BEF_POINTS_AVAILABLE',ord(pcSetup),'1,2',GetShortDateTime(MemDateFrom),GetShortDateTime(MemDateTo)]));
        // Subs
        dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[tblName,'POINTS_VALUE',ord(pcSubPaidOffice),'3,6',GetShortDateTime(MemDateFrom),GetShortDateTime(MemDateTo)]));
        // Deleted
        dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[tblName,'POINTS_AVAILABLE',ord(pcDeletedMember),'9,10',GetShortDateTime(MemDateFrom),GetShortDateTime(MemDateTo)]));
        dm.IBCommitRetain;
        
        // select from tblTempName + sums
        dm.qryReport.Close;
        dm.qryReport.SQL.Text := format(SQL3,[tblName]);
        dm.qryReport.Open;

        dm.ibdsPointsAudit.Open;
        
        // need to include all the dates from the 2 date ranges
        if MMDateTimeFrom < DateFrom then
           DateFrom := MMDateTimeFrom;
        if MMDateTimeTo > DateTo then
           DateTo := MMDateTimeTo;
           
        LoadPointsAuditWithDates( DateFrom,DateTo);

        with dm.qryReport do begin
           close;
           SQL.Text := 'Select * from POINTS_AUDIT ORDER BY EDIT_DATE';
           open;
        end;
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        result := True;
     finally
        tblTemp.Close;
        tblTemp.Free;
        dm.ibdsPointsAudit.Close;
     end;   
end;
{ ****************************************************************************}
procedure TfrmPointsHistory.LoadPointsAuditWithDates( DateFrom,DateTo : TDateTime);
var i,intDateFrom,intDateTo : Integer;
    v : Variant;
begin
     // fill up with the date Range
     intDateFrom := trunc(DateFrom);
     intDateTo   := trunc(DateTo);
     for i := intDateFrom to intDateTo do begin
        dm.ibdsPointsAudit.Insert;
        dm.ibdsPointsAudit.FieldByName('POINTS_AUDIT_ID').AsInteger := 0;
        dm.ibdsPointsAudit.FieldByName('LOCATION').AsString     := IntToStr(i);
        dm.ibdsPointsAudit.FieldByName('EDIT_DATE').AsDateTime  := DateFrom;
        dm.ibdsPointsAudit.Post; 
        DateFrom := DateFrom + 1;
     end;
     dm.ibdsPointsAudit.Transaction.CommitRetaining;
     // must refresh here else problems down the track
     dm.ibdsPointsAudit.Close;
     dm.ibdsPointsAudit.Open;
     dm.ibdsPointsAudit.First;
     dm.qryReport.First;
     while not dm.qryReport.EOF do begin
        // find the date
        v := dm.qryReport.FieldByName('TIME_STAMP').AsDateTime;
        if dm.ibdsPointsAudit.Locate('EDIT_DATE',v,[]) then begin
           //showmessage('qryReport - ' + DateTimeToStr( dm.qryReport.FieldByName('EDIT_DATE').AsDateTime) + '  IBTable - ' + DateTimeToStr(IBTable.FieldByName('EDIT_DATE').AsDateTime) + ' ' + dm.qryReport.FieldByName('BONUS_POINTS').AsString);
           dm.ibdsPointsAudit.Edit;
           dm.ibdsPointsAudit.FieldByName('LOCATION').AsString := 'Found';
           case dm.qryReport.FieldByName('PROCESS').AsInteger of
           -2 : begin
                   dm.ibdsPointsAudit.FieldByName('POINTS_EARNT').AsFloat := dm.qryReport.FieldByName('POINTS_EARNT').AsFloat;
                   dm.ibdsPointsAudit.FieldByName('POINTS_SPENT').AsFloat := dm.qryReport.FieldByName('POINTS_SPENT').AsFloat;
                end;   
           -1 : dm.ibdsPointsAudit.FieldByName('VIP_CREDIT').AsFloat      := dm.qryReport.FieldByName('POINTS_EARNT').AsFloat;
           2  : dm.ibdsPointsAudit.FieldByName('ADJUSTED_POINTS').AsFloat := dm.qryReport.FieldByName('POINTS_EARNT').AsFloat;
           3  : dm.ibdsPointsAudit.FieldByName('SUBS_POINTS').AsFloat     := dm.qryReport.FieldByName('POINTS_EARNT').AsFloat;
           10 : dm.ibdsPointsAudit.FieldByName('DELETED_POINTS').AsFloat  := dm.qryReport.FieldByName('POINTS_EARNT').AsFloat;
           end;
           dm.ibdsPointsAudit.Post;
        end;   
        dm.qryReport.Next;
     end;   
     
end;
{ ****************************************************************************}
procedure TfrmPointsHistory.LoadPointsAudit(var tblTemp : TIBTable);
begin
     with tblTemp do begin
        dmMM.qryMMReport.First;
        while not dmMM.qryMMReport.EOF do begin
           Insert;
           FieldByName('ITEM_NAME').AsString    := dmMM.qryMMReport.FieldByName('ITEM_NAME').AsString;
           FieldByName('TIME_STAMP').AsDateTime := dmMM.qryMMReport.FieldByName('TIME_STAMP').AsDateTime;
           FieldByName('POINTS_EARNT').AsFloat  := dmMM.qryMMReport.FieldByName('POINTS_EARNED').AsFloat;
           FieldByName('POINTS_SPENT').AsFloat  := dmMM.qryMMReport.FieldByName('POINTS_REDEEMED').AsFloat;
           FieldByName('PROCESS').AsInteger     := -2;
           Post;
           dmMM.qryMMReport.Next;
        end;
     end;     
end;
{ ****************************************************************************}
                            

end.

     
