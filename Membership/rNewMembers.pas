unit rNewMembers;
{
  New Member and Monthly Subscriptions Due use this report
  Member Ages , New Members by Joined date
}
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;
type
    TReportTypes  = (rtNewMemberRpt,rtMonthlySubsRpt,rtMemberAgesRpt,rtNewMemberJoinedRpt);
type
  TfrmNewMember = class(TForm)
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
    QRLabel16: TQRLabel;
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText3: TQRDBText;
    QRDBText8: TQRDBText;
    QRDBText9: TQRDBText;
    QRLabel1: TQRLabel;
    lblActive: TQRLabel;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRLabel2: TQRLabel;
    lblFinancial: TQRLabel;
    QRLabel3: TQRLabel;
    QRLabel5: TQRLabel;
    lblAddress: TQRLabel;
    QRLabel7: TQRLabel;
    QRDBText4: TQRDBText;
    lblDOB: TQRLabel;
    procedure QuickRepBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    function Extract( qrPrint,NoDOB : Boolean;PrtIndex : shortInt ;mGroups : ShortString; 
                      DateFrom,DateTo : TDateTime ; ReportTypes : TReportTypes ; YearFrom,YearTo : Smallint) : Boolean;
    procedure DetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
  private
    { Private declarations }
    Printing : Boolean;     
  public
    { Public declarations }
  end;
  

var
  frmNewMember: TfrmNewMember;
  

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
{ **************************************************************************** }
procedure TfrmNewMember.QuickRepBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;
{ **************************************************************************** }
function TfrmNewMember.Extract( qrPrint,NoDOB : Boolean;PrtIndex : shortint;mGroups : ShortString; 
                                DateFrom,DateTo : TDateTime ; ReportTypes : TReportTypes ; YearFrom,YearTo : Smallint ) : Boolean;
const PreSQL = 'SELECT m.MEMBER,m.FULL_NAME,m.FIRST_NAME,m.KNOWN_AS,m.JOINED,m.EXPIRES,m.IS_ACTIVE,m.SUB_PAID,' +
               'm.DATE_OF_BIRTH,m.HOME_PHONE,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY FROM MEMBERS m ';
      SQL1 = '%s WHERE m.SAVED = ''N''';
      SQL2 = ' %s WHERE m.EXPIRES BETWEEN  ''%s'' AND ''%s'' and m.DECEASED <> ''Y'' ' +
             'and (SELECT SUM(AMOUNT) from M_SUBS MS where MEMBER = MS.MEMBER)  > 0.00';
      SQL3 = 'where (extract(Year from Current_date) - Extract(Year from m.DATE_OF_BIRTH)) ' +
             'BETWEEN %d AND %d';       
      SQL4 = 'where m.DATE_OF_BIRTH is null or m.DATE_OF_BIRTH = ''12/30/1899''';
      SQL5 = 'where m.JOINED BETWEEN ''%s'' AND ''%s''';       
begin    
     result := False;
     with dm.qryReport do begin
         close;
         case ReportTypes of 
         rtNewMemberRpt : begin
                       if mGroups <> '' then begin
                          SQL.Text := preSQL + format(SQL1,[',M_GROUPS mg ']);
                          SQL.Add(Format(' and m.MEMBER = mg.MEMBER AND mg.GROUPS in (%s)',[mGroups]));
                       end else begin
                          SQL.Text := preSQL + format(SQL1,['']);
                       end;
                       end;
         rtMonthlySubsRpt : begin
                       if mGroups <> '' then begin
                          SQL.Text := preSQL + format(SQL2,[',M_GROUPS mg ',GetShortDAte(DateFrom),GetShortDAte(DateTo)]);
                          SQL.Add(Format(' and m.MEMBER = mg.MEMBER AND mg.GROUPS in (%s)',[mGroups]));
                       end else begin
                          SQL.Text := preSQL + format(SQL2,['',GetShortDAte(DateFrom),GetShortDAte(DateTo)]);
                       end;
                       end;      
         rtMemberAgesRpt : begin
                           case NoDOB of
                              true : SQL.Text  := PreSQL + SQL4; 
                              false : SQL.Text := PreSQL + format(SQL3,[YearFrom,YearTo]); 
                           end;
                       end;
         rtNewMemberJoinedRpt : begin
                           SQL.Text := preSQL + format(SQL5,[GetShortDAte(DateFrom),GetShortDAte(DateTo)]);
                       {if mGroups <> '' then begin
                          SQL.Text := preSQL + format(SQL1,[',M_GROUPS mg ']);
                          SQL.Add(Format(' and m.MEMBER = mg.MEMBER AND mg.GROUPS in (%s)',[mGroups]));
                       end else begin
                          SQL.Text := preSQL + format(SQL1,['']);}
                       end;
         end;             
         SQL.Add(' ORDER BY m.MEMBER');
         open;
     end;
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ **************************************************************************** }
procedure TfrmNewMember.DetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
var YesNoTypes : TYesNoTypes;
begin
     with dm.qryReport do begin
        if (FieldByName('DATE_OF_BIRTH').IsNull) or 
           (DateTimeToStr(FieldByName('DATE_OF_BIRTH').AsDateTime) = StrToDate1899) then begin
           lblDOB.Caption := '';
        end else begin
           lblDOB.Caption := GetShortMonthDate(FieldByName('DATE_OF_BIRTH').AsDAteTime);
        end;
        if FieldByName('IS_ACTIVE').AsString = varYes then
           lblActive.Caption := varYesWord
        else
           lblActive.Caption := varNoWord;
        YesNoTypes := TYesNoTypes( FieldByName('SUB_PAID').AsInteger);
        lblFinancial.Caption := YesNoTypesArray[ord(YesNoTypes)];
        lblAddress.Caption := '';
        lblAddress.Caption := GetAddressAsString(FieldByName('ADDRESS').AsString,
                                                 FieldByName('DISTRICT').AsString,
                                                 FieldByName('ADDRESS1').AsString,
                                                 FieldByName('CITY').AsString,
                                                 FieldByName('COUNTRY').AsString);
   end;  
end;
{ **************************************************************************** }
end.
