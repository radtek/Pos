unit rVotingList;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls,DB,IBCustomDataSet;

type
  TfrmVotingList = class(TForm)
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
    QRBand3: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText9: TQRDBText;
    QRDBText1: TQRDBText;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    QRDBText3: TQRDBText;
    QRDBText4: TQRDBText;
    QRDBText5: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    lblIndex: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    QRLabel7: TQRLabel;
    QRDBText6: TQRDBText;
    QRDBText7: TQRDBText;
    procedure QuickRepBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract( qrPrint : Boolean ; PrtIndex : SmallInt ;GroupsFields, tblName : string; byalpha : shortint ) : Boolean;
    procedure QRBand2BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    procedure qrStartPage(Sender: TCustomQuickRep);
    procedure QRBand3AfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
  private
    { Private declarations }
    FFirstMember,FLastMember : ShortString;
    ReportBy : ( rbSurname,rbMember );
    FRecordNum : ShortInt;
  public
    { Public declarations }

  end;

var
  frmVotingList: TfrmVotingList;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
var
   Printing : Boolean;
{ **************************************************************************** }
procedure TfrmVotingList.QuickRepBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;
{ **************************************************************************** }
function TfrmVotingList.Extract( qrPrint : Boolean ; PrtIndex : SmallInt ;GroupsFields,tblName : string; byalpha : shortint ) : Boolean;
// create a temp table , select it and put 4 records into 1 line in the ibdsVoteList
// then do a final select on this
var prtOrder : string[7];
    ibdsVoteList : TIBDataset;
const SQL1 = 'SELECT M.MEMBER,M.SURNAME,M.FIRST_NAME FROM MEMBERS M ' +
             'where M.SUB_PAID > 0 ORDER BY M.%s';
      SQL3 = 'SELECT M.MEMBER,M.SURNAME,M.FIRST_NAME FROM MEMBERS M,M_GROUPS mg ' +
             'where M.SUB_PAID > 0 and m.MEMBER = mg.MEMBER and mg.GROUPS in (%s) ORDER BY M.%s';
      SQL2 = 'CREATE TABLE %s ' +
             ' ( MEMBER1 INTEGER,SURNAME1 VARCHAR(25),' +
             '   MEMBER2 INTEGER,SURNAME2 VARCHAR(25),' +
             '   MEMBER3 INTEGER,SURNAME3 VARCHAR(25),' +
             '   MEMBER4 INTEGER,SURNAME4 VARCHAR(25))';
begin
     result := False;
     case byalpha of
        0 : begin
              prtOrder := 'SURNAME';
              ReportBy := rbSurname;
           end;
        1 : begin
              prtOrder := 'MEMBER';
              ReportBy := rbMember;
            end;
     end;
     try
        dm.qryOpen(dm.qryReport,format(SQL2,[tblName]));
        dm.qryReport.Close;
        dm.IBCommitRetain;
        if GroupsFields = '' then
           dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[prtOrder]))
        else   
           dm.sqlExecQuery(dm.qryGeneral,format(SQL3,[GroupsFields,prtOrder]));
        ibdsVoteList := TIBDataset.create(nil);
        with ibdsVoteList do begin
            Database := dm.IBDatabase;
            Transaction := dm.IBTransaction;
            SelectSQL.Text := format('select MEMBER1,SURNAME1,MEMBER2,SURNAME2,MEMBER3,SURNAME3,MEMBER4,SURNAME4 ' +
                                     ' from %s',[tblName]);
            InsertSQL.Text := Format('insert into %s (MEMBER1,SURNAME1,MEMBER2,SURNAME2,MEMBER3,SURNAME3,MEMBER4,SURNAME4) ' +
                               ' values (:MEMBER1,:SURNAME1,:MEMBER2,:SURNAME2,:MEMBER3,:SURNAME3,:MEMBER4,' +
                               ':SURNAME4)',[tblName]);
            open;
            FRecordNum := 1;
            while not dm.qryGeneral.EOF do begin
               if FRecordNum = 1 then
                  insert;
               FieldByName('MEMBER'  + intToStr(FRecordNum)).AsInteger := dm.qryGeneral.FieldByName('MEMBER').AsInteger;
               FieldByName('SURNAME' + intToStr(FRecordNum)).AsString := dm.qryGeneral.FieldByName('SURNAME').AsString + ' ' + dm.qryGeneral.FieldByName('FIRST_NAME').AsString;
               dm.qryGeneral.Next;
               if FRecordNum = 4 then begin
                  post;
                  FRecordNum := 1;
               end else begin
                  inc(FRecordNum);
               end;
            end;
            if ibdsVoteList.State = dsInsert then Post; // half record written
        end;
        dm.qryOpen(dm.qryReport ,format('SELECT * FROM %s',[tblName]));
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        Printing := qrPrint;
        Result := True;
     finally
        dm.qryGeneral.Close;
        ibdsVoteList.close;
        ibdsVoteList.free;
     end;
end;
{ **************************************************************************** }
procedure TfrmVotingList.QRBand2BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     lblIndex.Caption := format( '%s to %s',[Trim(FFirstMember),Trim(FLastMember)]);
end;
{ **************************************************************************** }
procedure TfrmVotingList.qrStartPage(Sender: TCustomQuickRep);
begin
     case ReportBy of
        rbMember  : FFirstMember := QRDBText9.DataSet.FieldByName('MEMBER1').AsString;
        rbSurname : FFirstMember := QRDBText9.DataSet.FieldByName('SURNAME1').AsString;
     end;
end;
{ **************************************************************************** }
procedure TfrmVotingList.QRBand3AfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
begin
     if not dm.qryrEPORT.EOF then begin
        case ReportBy of
           rbMember  : FLastMember := QRDBText3.DataSet.FieldByName('MEMBER4').AsString;
           rbSurname : FLastMember := QRDBText3.DataSet.FieldByName('SURNAME4').AsString;
        end;
     end;
end;
{ **************************************************************************** }

end.
