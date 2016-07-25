unit rMemberDraw;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls,IBTable,DB, Grids, Wwdbigrd, Wwdbgrid;

type
  TfrmMemberDraw = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    QRLabel10: TQRLabel;
    QRBand5: TQRBand;
    QRLabel8: TQRLabel;
    QRBand3: TQRBand;
    QRDBText9: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText6: TQRDBText;
    QRDBText7: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRLabel6: TQRLabel;
    QRLabel9: TQRLabel;
    QRLabel11: TQRLabel;
    QRLabel13: TQRLabel;
    QRLabel1: TQRLabel;
    QRLabel2: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel7: TQRLabel;
    QRLabel12: TQRLabel;
    QRLabel14: TQRLabel;
    QRLabel15: TQRLabel;
    QRLabel16: TQRLabel;
    QRLabel17: TQRLabel;
    QRDBText1: TQRDBText;
    QRDBText2: TQRDBText;
    QRDBText4: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText8: TQRDBText;
    QRDBText10: TQRDBText;
    QRDBText11: TQRDBText;
    QRDBText12: TQRDBText;
    lblReportTitle: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract(PrtIndex : Integer; tblName : string) : Boolean;
    
  private                             
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMemberDraw: TfrmMemberDraw;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
{ ****************************************************************************}
procedure TfrmMemberDraw.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
    //lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmMemberDraw.Extract(PrtIndex : Integer; tblName : string) : Boolean;
var i,j : integer;             
const SQL1 = 'select member from members where sub_paid > 0 and DECEASED <> ''Y'' and ' +
             'CARD_TYPE = ''N'' order by member';
begin
     try                          
        Result := False;
        // may have already been used
        if dm.cdsTemp.Active then begin
           dm.cdsTemp.EmptyDataSet;
           dm.cdsTemp.Active := False;
        end;
        try
           with dm.cdsTemp.FieldDefs do begin
              Clear;
              Add('DRAW1', ftInteger, 0);
              Add('DRAW2', ftInteger, 0);
              Add('DRAW3', ftInteger, 0);
              Add('DRAW4', ftInteger, 0);
              Add('DRAW5', ftInteger, 0);
              Add('DRAW6', ftInteger, 0);
              Add('MEMBER1', ftInteger, 0);
              Add('MEMBER2', ftInteger, 0);
              Add('MEMBER3', ftInteger, 0);
              Add('MEMBER4', ftInteger, 0);
              Add('MEMBER5', ftInteger, 0);
              Add('MEMBER6', ftInteger, 0);
           end;
           dm.cdsTemp.CreateDataSet;
        except
           showmessage('Create table failed');
        end;
        dm.cdsTemp.LogChanges := False;
        with dm.qryGeneral do begin
           dm.sqlExecQuery(dm.qryGeneral,SQL1);
           i := 1;  j := 1;
           while not EOF do begin
              if j = 1 then
                 dm.cdsTemp.Append;
              dm.cdsTemp.FieldByName('DRAW' + IntToStr(j)).AsInteger := i; inc(i);
              dm.cdsTemp.FieldByName('MEMBER'+ IntToStr(j)).AsInteger := FieldByName('MEMBER').AsInteger;
              next;
              if j = 6 then begin
                 dm.cdsTemp.Post;
                 j := 1;
              end else begin
                 inc(j);
              end;
           end;
            if dm.cdsTemp.State = dsInsert then dm.cdsTemp.Post; // half record written
        end;
        dm.cdsTemp.First;
        Result := True;
     finally
     end;   
end;
{ ****************************************************************************}

end.
