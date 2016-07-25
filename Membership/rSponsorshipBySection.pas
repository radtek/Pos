unit rSponsorshipBySection;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls,IBTable;

type
  TfrmSponsorshipBySection = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand5: TQRBand;
    QRLabel12: TQRLabel;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    qrDetailBand: TQRBand;
    QRDBText4: TQRDBText;
    QRDBText6: TQRDBText;
    QRDBText7: TQRDBText;
    qrSummaryBand: TQRBand;
    QRExpr6: TQRExpr;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRLabel3: TQRLabel;
    QRDBText8: TQRDBText;
    procedure qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
    function Extract(PrtIndex : shortint; DateFrom,DateTo : TDAteTime; tblName,Locations : string  ) : Boolean;
    function ExtractMember(PrtIndex : shortint; DateFrom,DateTo : TDAteTime ;tblName,Locations : string   ) : Boolean;
    procedure LoadTable(var tblTemp : TIBTable);
    procedure LoadTableMember(var tblTemp : TIBTable);
    
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmSponsorshipBySection: TfrmSponsorshipBySection;

implementation

{$R *.DFM}
uses Global,
     dmMembers,
     dmMenuMate;
{ ****************************************************************************}
procedure TfrmSponsorshipBySection.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmSponsorshipBySection.Extract(PrtIndex : shortint; DateFrom,DateTo : TDAteTime ;tblName,Locations : string   ) : Boolean;
var  tblTemp : TIBTable;
     MMDateFrom,MMDateTo : TDateTime;
     s,s1,Members : String;
     TotalSales : Extended;
const
      MMsql =
		'Select C.Contacts_3rdParty_Key as MEMBER,Sum(A.PRICE) as SALES ' +
		'From Archive A Inner Join Security S On A.Security_Ref = S.Security_Ref ' +
      'Inner Join Contacts C on A.Loyalty_Key = C.Contacts_Key ' +
		'Where S.Time_Stamp between :DateFrom And :DateTo And S.Security_Event = ''Ordered By'' ' +
      'and  C.Contacts_3rdParty_Key in (%s ) %s ' +
		'Group By C.Contacts_3rdParty_Key Union All ' +
		'Select C.Contacts_3rdParty_Key as MEMBER,Sum(D.PRICE) as SALES ' +
		'From DayArchive D Inner Join Security S On D.Security_Ref = S.Security_Ref ' +
      'Inner Join Contacts C on D.Loyalty_Key = C.Contacts_Key ' +
		'Where S.Time_Stamp between :DateFrom And :DateTo And S.Security_Event = ''Ordered By'' ' +
      'and  C.Contacts_3rdParty_Key in ( %s ) %s ' +
		'Group By C.Contacts_3rdParty_Key ';

      SQL1 = 'CREATE TABLE %s ( MEMBER Integer,FULL_NAME varCHAR(60),SALES Numeric(16,4),PERCENT Numeric(16,4))';

      SQL2 = 'Insert into %s select MEMBER,FULL_NAME,0,0 from members where CARD_TYPE = ''S''';

      SQL3 = 'Select MEMBER from %s';
             
      SQL4 = 'Select sum(SALES) as SALES from %s';

      SQL5 = 'update %s set PERCENT = (SALES / %f) * 100'; 
      
      SQLLocations  = ' and A.ORDER_LOCATION in ( %s ) ';
      SQLLocations1 = ' and D.ORDER_LOCATION in ( %s ) ';
      
begin                    
     result := False;
     try
        // create table
        dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[tblName]));
        dm.qryGeneral.Close;                 
        dm.IBCommitRetain;
        // create temp table
        tblTemp := TIBTable.create(nil); 
        tblTemp.Database    := dm.IBDatabase;
        tblTemp.Transaction := dm.IBTransaction;
        tblTemp.Tablename   := tblName;
        tblTemp.Close;

        // Add Members with Card Type of S ( Section Card )
        dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[tblName]));
        // Get Members from tblName
        Members := '';
        dm.sqlExecQuery(dm.qryGeneral,format(SQL3,[tblName]));
        while not dm.qryGeneral.EOF do begin
           Members := Members + dm.qryGeneral.FieldByName('MEMBER').AsString + ',';
           dm.qryGeneral.Next;
        end;                  
        Members := DeleteLastComma(Members);                       
        if Members = '' then begin
           showmessage('There are no Members with a Card Type of Section Card');
           exit;
        end;
        // get data from MenuMate
        dmMM.qryMMReport.close;
        MMDateFrom  := DateFrom + 0.21;  // 5 am that day - same as the MenuMate reports
        MMDateTo    := DateTo + 1.21;    // 5 am on the next day
        if Locations <> '' then begin
           s := format(SQLLocations,[Locations]);
           s1 := format(SQLLocations1,[Locations]);
           dmMM.qryMMReport.SQL.Text := format(mmSQL,[Members,s,Members, s1]);
        end else begin
           dmMM.qryMMReport.SQL.Text := format(mmSQL,[Members,'',Members, '']);
        end;
        dmMM.qryMMReport.ParamByName('DateFrom').AsDateTime := MMDateFrom;
        dmMM.qryMMReport.ParamByName('DateTo').AsDateTime   := MMDateTo;
        dmMM.qryMMReport.open;
        LoadTable(tblTemp);
        dmMM.qryMMReport.Close;
        dmMM.MMTransaction.Commit;
        
        // Get Total Sales
        dm.sqlExecQuery(dm.qryGeneral,format(SQL4,[tblName]));
        TotalSales := dm.qryGeneral.FieldByName('SALES').AsFloat; 
        if TotalSales = 0.00 then begin
           showmessage('There are no Sales for the Sections');
           exit;
        end;
        TotalSales := trunc(TotalSales * 1000);
        TotalSales := TotalSales / 1000;
        // Update Percentages
        dm.sqlExecQuery(dm.qryGeneral,format(SQL5,[tblName,TotalSales]));
        
        // select from tblTempName 
        with dm.qryReport do begin
           close;
           SQL.Text := format('Select * from %s order by SALES desc',[tblName]);
           open;
        end;
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        result := True;
     finally
        tblTemp.Close;
        tblTemp.Free;
     end;   
end;
{ ****************************************************************************}
procedure TfrmSponsorshipBySection.LoadTable(var tblTemp : TIBTable);
var  v : Variant;
begin
     tblTemp.Open;
     dmMM.qryMMReport.First;
     while not dmMM.qryMMReport.EOF do begin
        v := dmMM.qryMMReport.FieldByName('MEMBER').AsInteger;
        if tblTemp.Locate('MEMBER',v,[]) then begin
           tblTemp.Edit;
           tblTemp.FieldByName('SALES').AsFloat := dmMM.qryMMReport.FieldByName('SALES').AsFloat;
           tblTemp.Post;
        end;   
        dmMM.qryMMReport.Next;
     end;
     tblTemp.Close;
     dm.IBCommitRetain;
end;
{ ****************************************************************************}
function TfrmSponsorshipBySection.ExtractMember(PrtIndex : shortint; DateFrom,DateTo : TDAteTime ;tblName,Locations : string   ) : Boolean;
var  tblTemp : TIBTable;
     MMDateFrom,MMDateTo : TDateTime;
     s,s1,Members : String;
     TotalSales : Extended;
const
      MMsql =
		'Select C.Contacts_3rdParty_Key as MEMBER,Sum(A.PRICE) as SALES ' +
		'From Archive A Inner Join Security S On A.Security_Ref = S.Security_Ref ' +
      'Inner Join Contacts C on A.Loyalty_Key = C.Contacts_Key ' +
		'Where S.Time_Stamp between :DateFrom And :DateTo And S.Security_Event = ''Ordered By'' ' +
      //'and  C.Contacts_3rdParty_Key in (%s ) %s ' +
		'%s Group By C.Contacts_3rdParty_Key Union All ' +
		'Select C.Contacts_3rdParty_Key as MEMBER,Sum(D.PRICE) as SALES ' +
		'From DayArchive D Inner Join Security S On D.Security_Ref = S.Security_Ref ' +
      'Inner Join Contacts C on D.Loyalty_Key = C.Contacts_Key ' +
		'Where S.Time_Stamp between :DateFrom And :DateTo And S.Security_Event = ''Ordered By'' ' +
     // 'and  C.Contacts_3rdParty_Key in ( %s ) %s ' +
		'%s Group By C.Contacts_3rdParty_Key ';

      SQL1 = 'CREATE TABLE %s ( MEMBER Integer,FULL_NAME varCHAR(60),SALES Numeric(16,4),PERCENT Numeric(16,4))';

      SQL2 = 'Insert into %s select SECTION,Description,0,0 from Sections';

             
      SQL4 = 'Select sum(SALES) as SALES from %s';

      SQL5 = 'update %s set PERCENT = (SALES / %f) * 100'; 
      
      SQLLocations  = ' and A.ORDER_LOCATION in ( %s ) ';
      SQLLocations1 = ' and D.ORDER_LOCATION in ( %s ) ';
      
begin                    
     result := False;
     try
        // create table
        dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[tblName]));
        dm.qryGeneral.Close;                 
        dm.IBCommitRetain;
        // create temp table
        tblTemp := TIBTable.create(nil); 
        tblTemp.Database    := dm.IBDatabase;
        tblTemp.Transaction := dm.IBTransaction;
        tblTemp.Tablename   := tblName;
        tblTemp.Close;
        // Add Sections
        dm.sqlExecQuery(dm.qryGeneral,format(SQL2,[tblName]));
        // get data from MenuMate
        dmMM.qryMMReport.close;
        MMDateFrom  := DateFrom + 0.21;  // 5 am that day - same as the MenuMate reports
        MMDateTo    := DateTo + 1.21;    // 5 am on the next day
        if Locations <> '' then begin
           s := format(SQLLocations,[Locations]);
           s1 := format(SQLLocations1,[Locations]);
           dmMM.qryMMReport.SQL.Text := format(mmSQL,[s, s1]);
        end else begin
           dmMM.qryMMReport.SQL.Text := format(mmSQL,['','']);
        end;
        dmMM.qryMMReport.ParamByName('DateFrom').AsDateTime := MMDateFrom;
        dmMM.qryMMReport.ParamByName('DateTo').AsDateTime   := MMDateTo;
        dmMM.qryMMReport.open;
        LoadTableMember(tblTemp);
        dmMM.qryMMReport.Close;
        dmMM.MMTransaction.Commit;
        // Get Total Sales
        dm.sqlExecQuery(dm.qryGeneral,format(SQL4,[tblName]));
        TotalSales := dm.qryGeneral.FieldByName('SALES').AsFloat; 
        if TotalSales = 0.00 then begin
           showmessage('There are no Sales for the Sections');
           exit;
        end;
        TotalSales := trunc(TotalSales * 1000);
        TotalSales := TotalSales / 1000;
        // Update Percentages
        dm.sqlExecQuery(dm.qryGeneral,format(SQL5,[tblName,TotalSales]));
        
        // select from tblTempName 
        with dm.qryReport do begin
           close;
           SQL.Text := format('Select * from %s order by SALES desc',[tblName]);
           open;
        end;
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        result := True;
     finally
        tblTemp.Close;
        tblTemp.Free;
     end;   
end;
{ ****************************************************************************}
procedure TfrmSponsorshipBySection.LoadTableMember(var tblTemp : TIBTable);
var  v : Variant;
     CntMember : LongInt;
     Sales : Currency;
const SQL1 = 'Select count(Member) as CntMember from m_Sections where member = %d';
      SQL2 = 'Select Section from m_Sections where member = %d';
begin
     tblTemp.Open;
     dmMM.qryMMReport.First;
     while not dmMM.qryMMReport.EOF do begin
        // get Members with the count of Sections - some members are in many sections
        // if they are in 3 sections then divide the Sales by 3
        dm.sqlExecQuery(dm.qryGeneral,format (SQL1,[dmMM.qryMMReport.FieldByName('MEMBER').AsInteger]));
        CntMember := dm.qryGeneral.FieldByName('CntMember').AsInteger;
        // Member has a Section/s
        if CntMember > 0 then begin
           Sales := dmMM.qryMMReport.FieldByName('SALES').AsFloat / CntMember;
           dm.sqlExecQuery(dm.qryGeneral,format (SQL2,[dmMM.qryMMReport.FieldByName('MEMBER').AsInteger]));
           // for Each Section
           while not dm.qryGeneral.Eof do begin
              v := dm.qryGeneral.FieldByName('SECTION').AsInteger;
              // Member is Section - multiuse report
              if tblTemp.Locate('MEMBER',v,[]) then begin
                 tblTemp.Edit;
                 tblTemp.FieldByName('SALES').AsFloat := tblTemp.FieldByName('SALES').AsFloat + Sales;
                 tblTemp.Post;
              end;   
              dm.qryGeneral.Next;
           end;   
        end;
        dmMM.qryMMReport.Next;
     end;   
     tblTemp.Close;
     dm.IBCommitRetain;
end;
{ ****************************************************************************}



end.

     
