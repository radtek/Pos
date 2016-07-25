unit rMemberList;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmMemberList = class(TForm)
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
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText1: TQRDBText;
    QRDBText3: TQRDBText;
    lblAddress: TQRLabel;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    QRDBText4: TQRDBText;
    procedure qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
    function Extract(  PrtIndex : Integer ; SortBy : ShortString ) : Boolean;
    procedure DetailBandBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private               
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMemberList: TfrmMemberList;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster


procedure TfrmMemberList.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmMemberList.Extract(  PrtIndex : Integer ; SortBy : ShortString ) : Boolean;
// select distinct - could be many subs or groups
const  SQL1 = 'SELECT distinct(m.MEMBER),m.FULL_NAME,m.FIRST_NAME,m.SURNAME,m.KNOWN_AS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY,' +
              'm.EMAIL,m.RECEIVE_EMAIL,g.DESCRIPTION,' +
              'm.HOME_PHONE,m.EXPIRES,m.SUB_PAID,m.MALE_FEMALE from MEMBERS m,M_GROUPS mg,GROUPS g ' +
              'where m.MEMBER = mg.MEMBER and mg.GROUPS = g.GROUPS and m.DECEASED <> ''Y'' ';
begin
     result    := False;
     with dm.qryReport do begin
         close;
         SQL.Text := SQL1;
         if SortBy <> '' then
            SQL.Add( Format( '%s',[SortBy]));
         //sm(SQl.TEXT);
         open;
         qr.PrinterSettings.PrinterIndex := PrtIndex;
         Result := True;
     end;
end;
{ ****************************************************************************}
procedure TfrmMemberList.DetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     lblAddress.Caption := '';
     with dm.qryReport do begin
        lblAddress.Caption := GetAddressAsString(FieldByName('ADDRESS').AsString,
                                                 FieldByName('DISTRICT').AsString,
                                                 FieldByName('ADDRESS1').AsString,
                                                 FieldByName('CITY').AsString,
                                                 FieldByName('COUNTRY').AsString);
    end;
end;
{ ****************************************************************************}
end.
