unit rSectionInform;
{  Report doubles up - Member by Section and Section by Member
   and Subs Information Report
}
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmSectionInform = class(TForm)
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
    QRLabel6: TQRLabel;
    QRLabel8: TQRLabel;
    DetailBand: TQRBand;
    dbtMember: TQRDBText;
    dbtFullName: TQRDBText;
    dbtPhone: TQRDBText;
    QRLabel2: TQRLabel;
    QRDBText1: TQRDBText;
    lblSection: TQRLabel;
    lblDesc: TQRLabel;
    dbtDesc: TQRDBText;
    dbtSection: TQRDBText;
    QRGroup: TQRGroup;
    lblGSection: TQRLabel;
    dbtGSection: TQRDBText;
    dbtGDesc: TQRDBText;
    lblPaid: TQRLabel;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    dbtSurname: TQRDBText;
    lblSurname: TQRLabel;
    QRBand3: TQRBand;
    QRLabel21: TQRLabel;
    QRExpr1: TQRExpr;
    GroupFooter: TQRBand;
    QRLabel4: TQRLabel;
    QRDBText3: TQRDBText;
    QRExpr2: TQRExpr;
    QRLabel9: TQRLabel;
    lblAddress: TQRLabel;
    lblUser: TQRLabel;
    dbtUser: TQRDBText;
    function Extract( qrPrint : Boolean; PrtIndex : shortint ;Section : shortstring; SortBy,Members : string ): Boolean;
    function ExtractSubs(  PrtIndex : ShortInt ;Subs,SortBy,Members : shortstring ): Boolean;
    procedure qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
    procedure DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    procedure DetailBandAfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
    function ExtractInterests(  PrtIndex : shortInt ): Boolean;

  private
    { Private declarations }
     FBySection : Boolean;
     FSortByMember,Printing : Boolean;
     LastMember : longInt;
  public
    { Public declarations }
     property SortByMember : Boolean Write FSortByMember;
  end;

var
  frmSectionInform: TfrmSectionInform;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
{ **************************************************************************** }
procedure TfrmSectionInform.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
     with qr do begin
        lblUser.Enabled := false;
        lblUser.Visible := false;
        dbtUser.Visible := false;
        lblSurname.Caption   := 'Surname';
        dbtSurname.DataField := 'SURNAME';
        // if the report is a Secion Information Report
        if FBySection then begin
           // Papanui Only for Section Report
           if (POS('Papanui',CompName) > 0) then begin
              lblUser.Enabled := true;
              lblUser.Visible := true;
              dbtUser.Visible := true;
              lblSurname.Caption   := 'Card No.';
              dbtSurname.DataField := 'BARCODE_NUMBER';
           end;
        end;
        if FSortByMember then begin // by Member
              QRGroup.Enabled := False;
              GroupFooter.Enabled := False;
              dbtSection.Enabled := True;
              dbtDesc.Enabled := True;
              lblSection.Enabled := True;
              lblDesc.Enabled := True;
        end else begin
              QRGroup.Enabled := True;
             // dbtSection.Enabled := False;
             // dbtDesc.Enabled := False;
             // lblSection.Enabled := False;
             // lblDesc.Enabled := False;
        end;
     end;
end;
{ **************************************************************************** }
function TfrmSectionInform.Extract( qrPrint : Boolean; PrtIndex : shortInt ;Section : Shortstring; SortBy,Members : string ): Boolean;
const SQL1= 'SELECT S.DESCRIPTION,MS.SECTION,MS.EXPIRES,MS.PAID,MS.SPONSORSHIP,MS.AMOUNT_PAID,M.COMPANY,M.BARCODE_NUMBER,' +
            'M.MEMBER,M.ADDRESS,M.ADDRESS1,M.DISTRICT,M.CITY,M.COUNTRY,M.HOME_PHONE,M.JOINED,M.FULL_NAME,M.FIRST_NAME,M.SURNAME,M.KNOWN_AS,M.EMAIL,M.RECEIVE_EMAIL FROM ' +
            'SECTIONS S ,M_SECTIONS MS , MEMBERS M ' +
            'WHERE M.MEMBER = MS.MEMBER AND S.SECTION = MS.SECTION';
begin
     result := False;
     FBySection := True;
     with dm.qryReport do begin
         close;
         SQL.Text := SQL1;
         if Section <> '' then
            SQL.Add( Format( ' and MS.SECTION in (%s)',[Section]));
         if Members <> '' then
            SQL.Add( Format( '%s',[Members]));
         SQL.Add( Format( '%s',[SortBy]));
         open;
     end;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     Printing := qrPrint;
     result := True;
     LastMember := -1;

end;
{ **************************************************************************** }
function TfrmSectionInform.ExtractInterests(  PrtIndex : shortInt ): Boolean;
const SQL1= 'SELECT S.DESCRIPTION,MS.SECTION,'''' as EXPIRES,''N'' as PAID, ''N'' as SPONSORSHIP,0.00 as AMOUNT_PAID,m.SUB_PAID,' +
            'M.COMPANY,M.BARCODE_NUMBER,M.MEMBER,M.ADDRESS,M.ADDRESS1,M.DISTRICT,M.CITY,M.COUNTRY,M.HOME_PHONE,M.JOINED,M.FULL_NAME,M.FIRST_NAME,M.SURNAME,M.KNOWN_AS,M.EMAIL,M.RECEIVE_EMAIL FROM ' +
            'SECTIONS S ,M_INTERESTS MS , MEMBERS M ' +
            'WHERE M.MEMBER = MS.MEMBER AND S.SECTION = MS.SECTION order by MS.SECTION';
begin
     result := False;
     FBySection := False;
     with dm.qryReport do begin
         close;
         SQL.Text := SQL1;
         open;
     end;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
     LastMember := -1;
     //QRGroup.ForceNewPage := True;
end;
{ **************************************************************************** }
function TfrmSectionInform.ExtractSubs( PrtIndex : ShortInt; Subs,SortBy,Members : shortstring ): Boolean;
const SQL1= 'SELECT M.MEMBER,S.DESCRIPTION,MS.SUBSCRIPTION,M.EXPIRES,M.SUB_PAID,null as SPONSORSHIP,MS.AMOUNT_PAID,' +
            'M.COMPANY,M.BARCODE_NUMBER,M.ADDRESS,M.ADDRESS1,M.DISTRICT,M.CITY,M.COUNTRY,M.HOME_PHONE,M.JOINED,M.FULL_NAME,M.FIRST_NAME,M.SURNAME,M.KNOWN_AS,M.EMAIL,M.RECEIVE_EMAIL FROM ' +
            'SUBSCRIPTIONS S ,M_SUBS MS , MEMBERS M ' +
            'WHERE M.MEMBER = MS.MEMBER AND S.SUBSCRIPTION = MS.SUBSCRIPTION %s';
begin
     result := False;
     FBySection := False;
     FSortByMember := False; // no option yet
     QRGroup.Expression    := 'qryReport.SUBSCRIPTION';
     lblGSection.Caption   := 'Subscription';
     dbtGSection.DataField := 'SUBSCRIPTION';
     //
     //dbtSection.DataField := 'SUBSCRIPTION';
     with dm.qryReport do begin
         close;
         SQL.Text := format(SQL1,[Members]);
         if Subs <> '' then
            SQL.Add( Format( ' and MS.SUBSCRIPTION in (%s)',[Subs]));
         //SortBy := ' ORDER by m.SURNAME'; add in for Grouping
         System.Insert('MS.SUBSCRIPTION,',SortBy,(pos('by',SortBy) + 3));
         SQL.Add( Format( '%s',[SortBy]));
         AddToClipboard(SQL.TEXT);
         open;
     end;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ **************************************************************************** }
procedure TfrmSectionInform.DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     // dont duplicate the members details if on more than one line
     if FSortByMember and FBySection then begin
        if LastMember = dm.qryReport.FieldByName('MEMBER').AsInteger then begin
           dbtPhone.Enabled    := False;
          // dbtAddress.Enabled  := False;
           dbtSurname.Enabled  := False;
           dbtMember.Enabled   := False;
           dbtFullName.Enabled := False;
        end else if FBySection then begin
           dbtPhone.Enabled    := True;
          // dbtAddress.Enabled  := True;
           dbtSurname.Enabled  := True;
           dbtMember.Enabled   := True;
           dbtFullName.Enabled := True;
        end;
     end;
     //if dm.qryReport.FieldByName('SPONSORSHIP').AsString = 'Y' then
     //   lblSpns.Caption := 'Yes'
     //else
     //   lblSpns.Caption := 'No';
     lblAddress.Caption := '';
     lblAddress.Caption := GetAddressAsString(dm.qryReport.FieldByName('ADDRESS').AsString,
                                              dm.qryReport.FieldByName('DISTRICT').AsString,
                                              dm.qryReport.FieldByName('ADDRESS1').AsString,
                                              dm.qryReport.FieldByName('CITY').AsString,
                                              dm.qryReport.FieldByName('COUNTRY').AsString);

     if FBySection then begin
        if dm.qryReport.FieldByName('PAID').AsString = varYes then
           lblPAid.Caption := YesNoTypesArray[ord(TYesNoTypes(ynYes))]
        else if dm.qryReport.FieldByName('PAID').AsString = varNo then
           lblPAid.Caption :=YesNoTypesArray[ord(TYesNoTypes(ynNo))]
        else 
           lblPAid.Caption :=YesNoTypesArray[ord(TYesNoTypes(ynPart))];
           
     end else begin
        lblPaid.CAption := YesNoTypesArray[ord(TYesNoTypes(dm.qryReport.FieldByName('SUB_PAID').AsInteger))];
     end;

end;
{ **************************************************************************** }
procedure TfrmSectionInform.DetailBandAfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
begin
     LastMember := dm.qryReport.FieldByName('MEMBER').AsInteger;
end;
{ **************************************************************************** }
end.
