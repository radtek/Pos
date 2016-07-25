unit rParentCode;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  QuickRpt, Qrctrls, ExtCtrls;

type
  TfrmParentCode = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand2: TQRBand;
    QRLabel1: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel5: TQRLabel;
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText1: TQRDBText;
    lblAddress: TQRLabel;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    QRBand5: TQRBand;
    QRExpr1: TQRExpr;
    QRLabel21: TQRLabel;
    qrGroup: TQRGroup;
    QRLabel6: TQRLabel;
    QRDBText4: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText6: TQRDBText;
    QRDBText8: TQRDBText;
    lblAddressGroup: TQRLabel;
    procedure qrBeforePrint( Sender: TCustomQuickRep; var PrintReport: Boolean );
    function  Extract( PrtIndex : Integer ) : Boolean;
    procedure qrGroupBeforePrint( Sender: TQRCustomBand; var PrintBand: Boolean );
    procedure DetailBandBeforePrint( Sender: TQRCustomBand; var PrintBand: Boolean );
    procedure GetAddress( theLabel : TQrLabel );
  private       
    { Private declarations }
    FMember : Longint;
  public
    { Public declarations }
  end;

var
  frmParentCode: TfrmParentCode;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global;    //StockMaster
{ ****************************************************************************}
procedure TfrmParentCode.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
end;
{ ****************************************************************************}
function TfrmParentCode.Extract(  PrtIndex : Integer ) : Boolean;
const  SQL1 = 'SELECT m.MEMBER,m.Mailing_List_Code,m.FULL_NAME,m.FIRST_NAME,m.SURNAME,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.KNOWN_AS,' +
              'm.CITY,m.COUNTRY,m.EMAIL,m.RECEIVE_EMAIL,m.HOME_PHONE,m.EXPIRES,m.SPARE_VARCHAR_50,' +
              'm.SUB_PAID,m.MALE_FEMALE from MEMBERS m where m.DECEASED <> ''Y'' and IS_ACTIVE = ''Y'' order by m.SPARE_VARCHAR_50 ';
begin
     with dm.qryReport do begin
        close;
        SQL.Text := SQL1;
        Open;
     end;
end;
{ ****************************************************************************}
procedure TfrmParentCode.qrGroupBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     FMember := dm.qryReport.FieldByName('MEMBER').AsInteger;
     GetAddress(lblAddressGroup);
end;
{ ****************************************************************************}
procedure TfrmParentCode.GetAddress( theLabel : TQrLabel );
begin
     with dm.qryReport do begin
        theLabel.Caption := '';
        theLabel.Caption := GetAddressAsString(FieldByName('ADDRESS').AsString,
                                                 FieldByName('DISTRICT').AsString,
                                                 FieldByName('ADDRESS1').AsString,
                                                 FieldByName('CITY').AsString,
                                                 FieldByName('COUNTRY').AsString);
     end;
end;
{ ****************************************************************************}
procedure TfrmParentCode.DetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     if FMember = dm.qryReport.FieldByName('MEMBER').AsInteger then 
        PrintBand := False;
     GetAddress(lblAddress);
end;                           
{ ****************************************************************************}

end.
