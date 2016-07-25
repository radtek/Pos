unit rInformation;
{
 this report is used for Members Information , Subs Age Report, Card Last Used Report
 and Clubs Report
}

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, QuickRpt, Qrctrls;

type
  TfrmInformation = class(TForm)
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
    lblPaidHeader: TQRLabel;
    DetailBand: TQRBand;
    QRDBText2: TQRDBText;
    QRDBText1: TQRDBText;
    QRDBText3: TQRDBText;
    lblPaid: TQRLabel;
    QRBand4: TQRBand;
    lblLogo: TQRLabel;
    lblExpires: TQRLabel;
    QRBand5: TQRBand;
    QRExpr1: TQRExpr;
    QRLabel21: TQRLabel;
    lblAddress: TQRLabel;
    qrGroup: TQRGroup;
    GroupFooter: TQRBand;
    QRLabel7: TQRLabel;
    QRExpr2: TQRExpr;
    QRLabel8: TQRLabel;
    QRDBText4: TQRDBText;
    QRLabel9: TQRLabel;
    QRLabel11: TQRLabel;
    QRLabel12: TQRLabel;
    QRLabel13: TQRLabel;
    lblMaleSubtotal: TQRLabel;
    lblMaleTotal: TQRLabel;
    lblFemaleSubtotal: TQRLabel;
    lblFemaleTotal: TQRLabel;
    QRLabel14: TQRLabel;
    QRLabel15: TQRLabel;
    lblNonTotal: TQRLabel;
    lblNonSubTotal: TQRLabel;
    QRLabel16: TQRLabel;
    QRDBText5: TQRDBText;
    QRLabel17: TQRLabel;
    QRDBText6: TQRDBText;
    function Extract( qrPrint : Boolean ;PrtIndex : Integer ; Group,Subs,SortBy,Members,MembersOther,MailJoin : ShortString ) : Boolean;
    function AgeExtract( PrtIndex : Integer ; Subs : ShortString ; DateFrom : TDateTime ) : Boolean;
    procedure qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
    procedure DetailBandBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    function CardExtract(  PrtIndex : Integer ; Groups,Members : ShortString ; DateFrom,DateTo : TDateTime ) : Boolean;
    function ClubExtract(  PrtIndex,mGender : Integer ; Members,ClubsFields,SortBy,SubsFields : ShortString ) : Boolean;
    procedure GroupFooterBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    procedure GroupFooterAfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
    procedure QRBand5BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
    function MaintExtract(  PrtIndex,Index : Integer ) : Boolean;
  private
    { Private declarations }
     ReportTypes    : (rtInformationRpt,rtSubsAgeRpt,rtCardLastUsedRpt,rtClubsRpt,rtMaintenanceRpt);
     NonSubtotalCtr,NonTotalCtr,FemaleSubTotalCtr,FemaleTotalCtr,MaleSubTotalCtr,MaleTotalCtr : smallint;
  public
    { Public declarations }
  end;

var
  frmInformation: TfrmInformation;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
var
   Printing : Boolean;

{ **************************************************************************** }
procedure TfrmInformation.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin
     lblHeading.Caption := Global.PrintLogo;
     NonSubTotalCtr  := 0;
     NonTotalCtr     := 0;
     FemaleSubTotalCtr := 0;
     FemaleTotalCtr  := 0;
     MaleSubTotalCtr := 0;
     MaleTotalCtr    := 0;
     case ReportTypes of
        rtInformationRpt,rtMaintenanceRpt  : lblPaidHeader.Caption := 'Paid';
        rtSubsAgeRpt      : lblPaidHeader.Caption := 'Date of Birth';
        rtCardLastUsedRpt : lblPaidHeader.Caption := 'Card Last Used';
        rtClubsRpt        : begin
                               qrGroup.Enabled := True;
                               lblPaidHeader.Caption := 'Gender';
                               GroupFooter.Enabled := True;
                            end;
     end;
end;
{ **************************************************************************** }
function TfrmInformation.Extract(  qrPrint : Boolean ;PrtIndex : Integer ; Group,Subs,SortBy,Members,MembersOther,MailJoin : ShortString ) : Boolean;
// select distinct - could be many subs or groups
const  SQL1 = 'SELECT distinct(m.MEMBER),m.FULL_NAME,m.INITIALS,m.FIRST_NAME,m.SURNAME,m.KNOWN_AS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY,' +
              'm.EMAIL,m.RECEIVE_EMAIL,' +
              'm.HOME_PHONE,m.EXPIRES,m.SUB_PAID,m.MALE_FEMALE from MEMBERS m %s where m.DECEASED <> ''Y'' ';
// uses subs and groups selection
begin
     result    := False;
     ReportTypes := rtInformationRpt;
     with dm.qryReport do begin
         close;
         if (Group <> '') and (Subs <> '') then
            SQL.Text := format(SQL1,[',M_GROUPS mg,M_SUBS ms'])
         else if Group <> '' then
            SQL.Text := format(SQL1,[',M_GROUPS mg'])
         else if Subs <> '' then
            SQL.Text := format(SQL1,[',M_SUBS ms'])
         else    
            SQL.Text := format(SQL1,['']);
         if Group <> '' then
            SQL.Add( Format( 'and m.MEMBER = mg.MEMBER and mg.GROUPS in (%s)',[Group]));
         if Subs <> '' then
            SQL.Add( Format( 'and m.MEMBER = ms.MEMBER and ms.SUBSCRIPTION in (%s)',[Subs]));
         if Members <> '' then
            SQL.Add( Format( '%s',[Members]));
         if MembersOther <> '' then 
            SQL.Add( Format( '%s',[MembersOther]));
         if MailJoin <> '' then 
            SQL.Add( Format( '%s',[MailJoin]));
         if SortBy <> '' then
            SQL.Add( Format( '%s',[SortBy]));
         //Showmessage(SQl.TEXT);
         open;
         qr.PrinterSettings.PrinterIndex := PrtIndex;
         Result := True;
     end;
end;
{ **************************************************************************** }
function TfrmInformation.AgeExtract(  PrtIndex : Integer ; Subs : ShortString ; DateFrom : TDateTime ) : Boolean;
const  SQL1 = 'SELECT m.MEMBER,m.FULL_NAME,m.FIRST_NAME,m.SURNAME,m.INITIALS,m.KNOWN_AS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY,' +
              'm.HOME_PHONE,m.EXPIRES,m.DATE_OF_BIRTH,m.MALE_FEMALE from MEMBERS m %s where m.DECEASED <> ''Y'' ' +
              'and m.DATE_OF_BIRTH <= ''%s''';
{ uses subs selection and an age >= x years from a date
  ie Junior Member >= 17 years at 31 March 2002}
begin
     result    := False;
     ReportTypes := rtSubsAgeRpt;
     with dm.qryReport do begin
         close;
         if Subs <> '' then begin
            SQL.Text := format(SQL1,[',M_SUBS ms',GetShortDate(DateFrom)]);
            SQL.Add( Format( 'and m.MEMBER = ms.MEMBER and ms.SUBSCRIPTION in (%s)',[Subs]));
         end else
            SQL.Text := format(SQL1,['',GetShortDate(DateFrom)]);   
         SQL.Add( 'order by m.DATE_OF_BIRTH,m.Member' );
         Global.AddToClipBoard(SQL.Text);
         open;
         qr.PrinterSettings.PrinterIndex := PrtIndex;
         Result := True;
     end;    
end;    
{ **************************************************************************** }
function TfrmInformation.CardExtract(  PrtIndex : Integer ; Groups,Members : ShortString ; DateFrom,DateTo : TDateTime ) : Boolean;
const  SQL1 = 'SELECT m.MEMBER,m.FULL_NAME,m.FIRST_NAME,m.SURNAME,m.INITIALS,m.KNOWN_AS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY,' +
              'm.HOME_PHONE,m.EXPIRES,m.CARD_LAST_USED,m.MALE_FEMALE from MEMBERS m %s where m.DECEASED <> ''Y'' ' +
              'and m.CARD_LAST_USED BETWEEN ''%s'' and ''%s''';
{ Card last used report }
begin
     result    := False;
     ReportTypes := rtCardLastUsedRpt;
     with dm.qryReport do begin
         close;
         if Groups <> '' then begin
            SQL.Text := format(SQL1,[',M_GROUPS mg',GetShortDate(DateFrom),GetShortDate(DateTo)]);
            SQL.Add( Format( 'and m.MEMBER = mg.MEMBER and mg.GROUPS in (%s)',[Groups]));
         end else
            SQL.Text := format(SQL1,['',GetShortDate(DateFrom),GetShortDate(DateTo)]);
         if Members <> '' then
            SQL.Add( Members );
         SQL.Add( 'order by m.CARD_LAST_USED,m.Member' );
         open;
         qr.PrinterSettings.PrinterIndex := PrtIndex;
         Result := True;
     end;
end;
{ **************************************************************************** }
function TfrmInformation.ClubExtract(  PrtIndex,mGender : Integer ; Members,ClubsFields,SortBy,SubsFields : ShortString ) : Boolean;
const  SQL1 = 'SELECT m.CLUBS,m.MEMBER,m.FULL_NAME,m.FIRST_NAME,m.SURNAME,m.KNOWN_AS,m.INITIALS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY,' +
              'm.HOME_PHONE,m.EXPIRES,m.CARD_LAST_USED,c.DESCRIPTION,m.MALE_FEMALE from MEMBERS m, CLUBS c %s where m.DECEASED <> ''Y'' ' +
              'and m.CLUBS = c.CLUB_ID';
begin
     result    := False;
     ReportTypes := rtClubsRpt;
     with dm.qryReport do begin
         close;
         // Subs
         if SubsFields = '' then begin
            SQL.Text := format(SQL1,['']);
         end else begin
            SQL.Text := format(SQL1,[' ,M_SUBS ms']);
            SQL.Add( format(' and m.MEMBER = ms.MEMBER and ms.SUBSCRIPTION in (%s)',[SubsFields])); // Men
         end;
         case mGender of
            0 : ; // All
            1 : SQL.Add( ' and m.MALE_FEMALE = ''M''' ); // Men
            2 : SQL.Add( ' and m.MALE_FEMALE = ''F''' ); // WoMen
         end;
         if Members <> '' then
            SQL.Add( Members );
         if ClubsFields <> '' then
            SQL.Add( format(' and CLUBS in (%s)',[ClubsFields] ));
         // Grouping is on
         System.Insert('M.CLUBS,',SortBy,(pos('by',SortBy) + 3));
         SQL.Add( SortBy );
         AddToClipboard(SQL.Text);
         open;
         qr.PrinterSettings.PrinterIndex := PrtIndex;
         Result := True;
     end;
end;
{ **************************************************************************** }
function TfrmInformation.MaintExtract(  PrtIndex,Index : Integer ) : Boolean;
const SQLGroups = 'select m.member,mg.GROUPS as LINK,m.FULL_NAME,m.EXPIRES,m.MALE_FEMALE,' + 
                  'm.FIRST_NAME,m.SURNAME,m.INITIALS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.SUB_PAID,m.COUNTRY,m.HOME_PHONE ' +
                  'from MEMBERS M LEFT JOIN M_GROUPS MG on m.MEMBER = MG.MEMBER order by mg.GROUPS,m.MEMBER';
      SQLSubs = 'select m.member,mg.SUBSCRIPTION as LINK,m.FULL_NAME,m.EXPIRES,m.MALE_FEMALE,' + 
                  'm.FIRST_NAME,m.SURNAME,m.INITIALS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.SUB_PAID,m.COUNTRY,m.HOME_PHONE ' +
                  'from MEMBERS M LEFT JOIN M_SUBS MG on m.MEMBER = MG.MEMBER order by mg.SUBSCRIPTION,m.MEMBER';
      SQLManySubs = 'select m.member,null as LINK,m.FULL_NAME,m.EXPIRES,m.MALE_FEMALE,' + 
                  'm.FIRST_NAME,m.SURNAME,m.INITIALS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.SUB_PAID,m.COUNTRY,m.HOME_PHONE ' +
                  'from MEMBERS M  where (select count(ms.member) from m_subs ms where m.member = ms.member) > 1 order by m.MEMBER';
      SQLManyGroups = 'select m.member,null as LINK,m.FULL_NAME,m.EXPIRES,m.MALE_FEMALE,' + 
                  'm.FIRST_NAME,m.SURNAME,m.INITIALS,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.SUB_PAID,m.COUNTRY,m.HOME_PHONE ' +
                  'from MEMBERS M  where (select count(ms.member) from m_Groups ms where m.member = ms.member) > 1 order by m.MEMBER';
begin
     result    := False;
     ReportTypes := rtMaintenanceRpt;
     with dm.qryReport do begin
         close;
         case Index of 
            65 : SQL.Text :=  SQLGroups;
            66 : SQL.Text :=  SQLSubs;
            70 : SQL.Text :=  SQLManyGroups;
            71 : SQL.Text :=  SQLManySubs;
         end;
         open;
         qr.PrinterSettings.PrinterIndex := PrtIndex;
         Result := True;
     end;
end;
{ ****************************************************************************}
procedure TfrmInformation.DetailBandBeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
var YesNoTypes : TYesNoTypes;
begin
     if ReportTypes = rtMaintenanceRpt then begin
        // count only for selected Members
        if dm.qryReport.FieldByName('LINK').IsNull then begin
           if dm.qryReport.FieldByName('MALE_FEMALE').IsNull then
              inc(NonSubtotalCtr)
           else if dm.qryReport.FieldByName('MALE_FEMALE').AsString = 'M' then
              inc(MaleSubtotalCtr)
           else   
              inc(FemaleSubtotalCtr);
        end;      
    end else begin
        if dm.qryReport.FieldByName('MALE_FEMALE').IsNull then
           inc(NonSubtotalCtr)
        else if dm.qryReport.FieldByName('MALE_FEMALE').AsString = 'M' then
           inc(MaleSubtotalCtr)
        else   
           inc(FemaleSubtotalCtr);
    end;       
        
     with dm.qryReport do begin
        case ReportTypes of
           rtInformationRpt : lblPaid.Caption := YesNoTypesArray[FieldByName('SUB_PAID').AsInteger];
           rtMaintenanceRpt : begin
                                 if not dm.qryReport.FieldByName('LINK').IsNull then 
                                    PrintBand := False;
                                 lblPaid.Caption := YesNoTypesArray[FieldByName('SUB_PAID').AsInteger];
                              end;   
           rtSubsAgeRpt     : begin
                                if FieldByName('DATE_OF_BIRTH').IsNull then
                                   lblPaid.Caption := ''
                                else
                                lblPaid.Caption := formatDateTime('dd/mm/yyyy',FieldByName('DATE_OF_BIRTH').AsDateTime);
                              end;
           rtCardLastUsedRpt : begin
                                if FieldByName('CARD_LAST_USED').IsNull then
                                   lblPaid.Caption := ''
                                else
                                lblPaid.Caption := GetShortMonthDate(FieldByName('CARD_LAST_USED').AsDateTime);
                              end;
           rtClubsRpt       : begin
                                if FieldByName('MALE_FEMALE').IsNull then
                                   lblPaid.Caption := ''
                                else begin
                                   if FieldByName('MALE_FEMALE').AsString = 'M' then begin
                                      lblPaid.Caption := 'Male'
                                   end else begin
                                      lblPaid.Caption := 'Female';
                                   end;
                                end;
                              end;
        end;  // case
        lblAddress.Caption := '';
        lblAddress.Caption := GetAddressAsString(FieldByName('ADDRESS').AsString,
                                                 FieldByName('DISTRICT').AsString,
                                                 FieldByName('ADDRESS1').AsString,
                                                 FieldByName('CITY').AsString,
                                                 FieldByName('COUNTRY').AsString);
        if FieldByName('EXPIRES').IsNull then
           lblExpires.Caption := ''
        else
           lblExpires.Caption := GetShortMonthDate(FieldByName('EXPIRES').AsDateTime);
     end;
end;
{ **************************************************************************** }
procedure TfrmInformation.GroupFooterBeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     lblFemaleSubTotal.Caption := IntToStr(FemaleSubTotalCtr);
     lblMaleSubTotal.Caption   := IntToStr(MaleSubTotalCtr);
     lblNonSubTotal.Caption    := IntToStr(NonSubTotalCtr);
     // add up for the summary band
     MaleTotalCtr   := MaleTotalCtr + MaleSubTotalCtr;
     FemaleTotalCtr := FemaleTotalCtr + FemaleSubTotalCtr;
     NonTotalCtr    := NonTotalCtr + NonSubTotalCtr;
end;
{ **************************************************************************** }
procedure TfrmInformation.GroupFooterAfterPrint(Sender: TQRCustomBand; BandPrinted: Boolean);
begin
     if GroupFooter.Enabled = true then begin
        FemaleSubTotalCtr := 0;
        MaleSubTotalCtr   := 0;
        NonSubTotalCtr    := 0;
     end;   
end;
{ **************************************************************************** }
procedure TfrmInformation.QRBand5BeforePrint(Sender: TQRCustomBand; var PrintBand: Boolean);
begin
     // totals are not accumulated
     if GroupFooter.Enabled = False then begin
        lblFemaleTotal.Caption := IntToStr(FemaleSubTotalCtr);
        lblMaleTotal.Caption   := IntToStr(MaleSubTotalCtr);
        lblNonTotal.Caption    := IntToStr(NonSubTotalCtr);
     end else begin
        lblFemaleTotal.Caption := IntToStr(FemaleTotalCtr);
        lblMaleTotal.Caption   := IntToStr(MaleTotalCtr);
        lblNonTotal.Caption    := IntToStr(NonTotalCtr);
     end;
end;
{ **************************************************************************** }

end.
