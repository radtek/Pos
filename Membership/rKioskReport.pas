unit rKioskReport;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls,DB;

type
  TfrmKioskReport = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand2: TQRBand;
    lblCode: TQRLabel;
    QRLabel8: TQRLabel;
    QRLabel9: TQRLabel;
    QRLabel11: TQRLabel;
    QRLabel13: TQRLabel;
    DetailBand: TQRBand;
    QRDBText1: TQRDBText;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText6: TQRDBText;
    QRDBText8: TQRDBText;
    QRBand4: TQRBand;
    QRExpr1: TQRExpr;
    QRExpr3: TQRExpr;
    QRExpr5: TQRExpr;
    QRExpr8: TQRExpr;
    QRLabel16: TQRLabel;
    QRBand5: TQRBand;
    lblLogo: TQRLabel;
    lblName: TQRLabel;
    QRDBText4: TQRDBText;
    FooterBand: TQRBand;
    GroupBand: TQRGroup;
    QuickRep1: TQuickRep;
    QRLabel3: TQRLabel;
    QRExpr4: TQRExpr;
    QRExpr6: TQRExpr;
    QRExpr7: TQRExpr;
    QRExpr9: TQRExpr;
    QRDBText5: TQRDBText;
    lblVisited: TQRLabel;
    procedure qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
    function Extract(PrtIndex,Summary,theDate : Integer) : Boolean;
    
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmKioskReport: TfrmKioskReport;


implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global;   //StockMaster
{ ****************************************************************************}
procedure TfrmKioskReport.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmKioskReport.Extract(PrtIndex,Summary,theDate : Integer) : Boolean;
var i,j : integer;             
    TotCtr : smallint;
    SaveGroupName : String[15];
    s : String[1];
    ADAte : TDateTime;
const SQL1 = 'select member,first_name,surname,KNOWN_AS,last_visit,MALE_FEMALE,extract(hour from LAST_VISIT) as VISIT_HOUR,' +
             'extract(minute from LAST_VISIT) as VISIT_MINUTE from members where LAST_VISIT between' +
             '''%s'' and ''%s'' order by LAST_VISIT';
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
              Add('GROUP', ftInteger, 0);
              Add('GROUP_NAME', ftString, 20);
              Add('MEMBER', ftInteger, 0);
              Add('NAME', ftString, 45);
              Add('MALE', ftBoolean, 0);
              Add('FEMALE', ftBoolean, 0);
              Add('NO_GENDER', ftBoolean, 0);
              Add('TOTAL', ftInteger, 0);
              Add('LAST_VISIT', ftDateTime, 0);
           end;
           dm.cdsTemp.CreateDataSet;
        except
           // doesnt like working under XP
           showmessage('Create table failed');
        end;

        dm.cdsTemp.LogChanges := False;

        with dm.qryGeneral do begin
           close;
           case theDAte of
              0 : ADAte := Date;    // Today
              1 : ADAte := (Date -1); // Yesterday
           end;   
           // add 1 to date
           // today first thing to tomorrow morning before 5am
           SQL.Text := format(SQL1,[formatdatetime('mm/dd/yyyy 05:00:00',ADAte),formatdatetime('mm/dd/yyyy 05:00:00',ADAte + 1)]);
           Execquery;
           i := 1; j := 1;
           TotCtr := 0;
           if FieldByName('VISIT_MINUTE').AsInteger < 30 then begin
              SaveGroupName := IntToStr(FieldByName('VISIT_HOUR').AsInteger) + ':00 - ' +
                               IntToStr(FieldByName('VISIT_HOUR').AsInteger) + ':30';
           end else begin
              SaveGroupName := IntToStr(FieldByName('VISIT_HOUR').AsInteger) + ':30 - ' +
                               IntToStr(FieldByName('VISIT_HOUR').AsInteger + 1) + ':00';
           end;                                                                    
           while not EOF do begin
              dm.cdsTemp.Append;
              if FieldByName('VISIT_MINUTE').AsInteger < 30 then begin
                 dm.cdsTemp.FieldByName('GROUP_NAME').AsString := IntToStr(FieldByName('VISIT_HOUR').AsInteger) + ':00 - ' +
                                                                  IntToStr(FieldByName('VISIT_HOUR').AsInteger) + ':30';
                 
              end else begin
                 dm.cdsTemp.FieldByName('GROUP_NAME').AsString := IntToStr(FieldByName('VISIT_HOUR').AsInteger) + ':30 - ' +
                                                                  IntToStr(FieldByName('VISIT_HOUR').AsInteger + 1) + ':00';
              end;
              // change from 9:00 to 9:30 etc
              if SaveGroupName <> dm.cdsTemp.FieldByName('GROUP_NAME').AsString then begin
                 SaveGroupName := dm.cdsTemp.FieldByName('GROUP_NAME').AsString; 
                 inc(i);
              end;
              dm.cdsTemp.FieldByName('GROUP').AsInteger     := i;
              dm.cdsTemp.FieldByName('MEMBER').AsInteger    :=  FieldByName('MEMBER').AsInteger;
              dm.cdsTemp.FieldByName('NAME').AsString :=    FieldByName('FIRST_NAME').AsString + ' ' +
                                                            FieldByName('SURNAME').AsString;
              if FieldByName('MALE_FEMALE').AsString = 'M' then
                 dm.cdsTemp.FieldByName('MALE').AsBoolean := True
              else if FieldByName('MALE_FEMALE').AsString = 'F' then
                 dm.cdsTemp.FieldByName('FEMALE').AsBoolean := True
              else   
                 dm.cdsTemp.FieldByName('NO_GENDER').AsBoolean := True;
              dm.cdsTemp.FieldByName('TOTAL').AsInteger := j;
              dm.cdsTemp.FieldByName('LAST_VISIT').AsDateTime := FieldByName('LAST_VISIT').AsDateTime;
              inc(j);
              dm.cdsTemp.Post;
              next;
           end;
        end;
        dm.cdsTemp.First;
        qr.PrinterSettings.PrinterIndex := PrtIndex;
        if Summary = 1 then begin
           GroupBand.Height := 1;
           DetailBand.Enabled := False;
           lblName.Enabled    := False;
           lblCode.Enabled    := False;
           lblVisited.Enabled := False;
        end;
        Result := True;
     finally
     end;
end;
{ ****************************************************************************}

end.
