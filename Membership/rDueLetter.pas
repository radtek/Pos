unit rDueLetter;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls, StdCtrls, ComCtrls,Printers, wwriched, jpeg;

type
  TfrmDueLetter = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRDBRichText1: TQRDBRichText;
    function Extract( qrPrint: Boolean; Whatby,PrtIndex : Integer; mGroups : shortstring;
                      DateFrom,DateTo : TDateTime; mFields,Lname : string ; YearFrom,YearTo : smallint) : Boolean;
    procedure DetailBand1BeforePrint(Sender: TQRCustomBand;      var PrintBand: Boolean);
    procedure DetailBand1AfterPrint(Sender: TQRCustomBand;      BandPrinted: Boolean);
    procedure GetSubString;
    procedure qrBeforePrint(Sender: TCustomQuickRep;   var PrintReport: Boolean);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
    FWhatBy : shortint;
    FNewLineList        : TStringList;
    FNewLineInsert : integer;
    FExportExtFieldsList : TStringList;             
    FStartStr : String;
    FEndStr   : String;
    FSaveLetter : String;
    FLine : string[40];
    FExportFieldsList : TStringList;
    MemberList : TStringList;
    MoreThanOneMember : Boolean;
    FSections : String[40];
    FDateFrom : String[12];
    FDateto   : String[12];
    FMonths   : Smallint;
    FRptBy    : SmallInt;
    FMembers  : String;
    procedure ProcExportExtFields( StrList : TStringList );
    procedure ProcExportFields( StrList : TStringList );
  public
    { Public declarations }
    property ExportExtFieldsList : TStringList Write ProcExportExtFields;
    property ExportFieldsList    : TStringList Write ProcExportFields;
    property RptBy       : SmallInt Write FRptBy;
    property RptMonth    : SmallInt Write FMonths;
    property RptMembers  : String Write FMembers;
    
  end;

var
  frmDueLetter: TfrmDueLetter;

implementation

{$R *.DFM}
uses
    Global,
    dmMembers;
{ **************************************************************************** }
procedure TfrmDueLetter.ProcExportExtFields( StrList : TStringList );
begin
     FExportExtFieldsList.Assign(StrList);
end;
{ **************************************************************************** }
procedure TfrmDueLetter.ProcExportFields( StrList : TStringList );
begin
     FExportFieldsList.Assign(StrList);
end;                    
{ **************************************************************************** }
function TfrmDueLetter.Extract( qrPrint : Boolean;Whatby,PrtIndex : Integer;mGroups: shortstring;
                                DateFrom,DateTo : TDateTime ; mFields,Lname : string 
                                ; YearFrom,YearTo : smallint) : Boolean;
var s : string;
const SQL1 = 'insert into Lucky_member SELECT M.MEMBER,m.mailing_list_code ' +
             'FROM MEMBERS M, SUBSCRIPTIONS S , M_SUBS ms %s WHERE M.EXPIRES BETWEEN  ''%s'' AND ''%s'' ' +
             'and m.DECEASED <> ''Y'' and m.MEMBER = MS.MEMBER  ' +
             'and MS.SUBSCRIPTION = s.SUBSCRIPTION and S.KEEP_EXPIRES_CURRENT <> ''Y'' %s';
            // reoved the bottom line 3/06/02 city club  (  and m.SUB_PAID > 0 )
            // 'and m.DECEASED <> ''Y'' and m.SUB_PAID > 0 and m.MEMBER = MS.MEMBER  ' +
      SQL2 = 'insert into Lucky_member SELECT M.MEMBER,m.mailing_list_code ' +
             'FROM MEMBERS M, M_SECTIONS MS WHERE m.member = ms.member ' +
             'and MS.EXPIRES BETWEEN  ''%s'' AND  ''%s'' and MS.AMOUNT  > 0.00 and m.DECEASED <> ''Y''';

{ use Lucky_member table - delete.
 insert m.mailing_list_code into lucky_member - gets all members with subs/sections due.
 select distint(m.mailing_list_code) from it and the relevant fields for the letter.
 get the relevant letter.
 }
begin
     result := False;
     FWhatBy := WhatBy;
     with dm.qryReport do begin
         if FWhatBy < 2 then begin  // only for subs sections Due  letter
            close;
            // clear table
            SQL.Text := 'delete from lucky_member';
            open;
         end;
         case FWhatBy of
            0 : begin // subs Due letter   - dont get life members
                   Close;
                   // Members who dont have a subscription get a price of 0.00
                   if mGroups = '' then begin
                      SQL.Text := Format( SQL1,['',GetShortDate(DateFrom),GetShortDate(DateTo),'']);
                   end else begin
                      s := Format('and m.MEMBER = Mg.MEMBER AND Mg.GROUPS in (%s)',[mGroups]);
                      SQL.Text := Format( SQL1,[',M_GROUPS mg',GetShortDate(DateFrom),GetShortDate(DateTo),s]);                   
                   end;
                   //sm(SQL.Text);
                   open;
                   dm.IBCommitRetain;
                   Application.ProcessMessages;
                   close;
                   SQL.Text := Format('select distinct(lm.Member),%s from Lucky_member lm ' +
                     ',members m where lm.member = m.member ' +
                     'ORDER BY LM.MEMBER',[mFields]);
                   open;
                end;      
            1 : begin // Sections Due letter   
                   Close;
                   SQL.Text := Format(SQL2,[GetShortDate(DateFrom),GetShortDate(DateTo)]);
                   if mGroups <> '' then SQL.Add(Format(' AND MS.SECTION in (%s)',[mGroups]));
                   open;
                   dm.IBCommitRetain;
                   Application.ProcessMessages;
                   close;
                   SQL.Text := Format('select distinct(lm.Member),%s from Lucky_member lm ' +
                     ',members m where lm.member = m.member  ' +
                     ' ORDER BY LM.MEMBER',[mFields]);
                   fetchall;
                   open;
                end;      
              2 : begin // Events letter   
                     inc(FMonths); // 0 based array
                     with dm.qryReport do begin
                        close;
                        SQL.Text :=  format('SELECT M.MEMBER,%s,ME.DATES,' +
                           'ME.EVENT,E.DESCRIPTION from MEMBERS M,M_EVENTS ME,EVENTS E ' +
                           'where M.MEMBER = ME.MEMBER AND ME.EVENT = E.EVENT and m.DECEASED <> ''Y'' ' +
                           'and extract( month from ME.DATES) = %d',[mFields,FMonths]);
                        if mGroups <> ''  then SQL.Add( Format( ' AND ME.EVENT in ( %s )',[mGroups]));
                        if FMembers <> '' then SQL.Add( Format( ' %s',[FMembers]));
                        SQL.Add( ' ORDER by M.MEMBER');
                        open;
                     end;
                end;
              3 : begin   // Selective Letter - Groups Sections or Subs
                   case FRptBy of
                      0 : begin   // groups
                             with dm.qryReport do begin
                                close;
                                SQL.Text :=  format('SELECT M.MEMBER,%s from MEMBERS M, M_GROUPS mg where ' +
                                                    'm.MEMBER = mg.MEMBER and m.DECEASED <> ''Y'' ',[mFields]);
                                if mGroups <> ''  then SQL.Add( Format( 'and mg.GROUPS in ( %s )',[mGroups]));
                                if FMembers <> '' then 
                                   SQL.Add( Format( ' %s',[FMembers]));
                                SQL.Add( ' ORDER by M.MEMBER');
                                addtoClipboard(SQL.TEXT);
                                //showmessage(SQL.TEXT);
                                
                                open;
                             end;
                          end;
                      1 : begin   // Sections
                             with dm.qryReport do begin
                                close;
                                SQL.Text :=  format('SELECT M.MEMBER,%s from MEMBERS M,' +
                                   'M_SECTIONS MS WHERE M.MEMBER = MS.MEMBER  and m.DECEASED <> ''Y'' ',[mFields]);
                                if mGroups <> ''  then SQL.Add( Format( ' AND MS.SECTION in ( %s )',[mGroups]));
                                if FMembers <> '' then SQL.Add( Format( ' %s',[FMembers]));
                                SQL.Add( ' ORDER by M.MEMBER');
                                open;
                             end;
                          end;
                      2 : begin   // Subs
                              with dm.qryReport do begin
                                close;
                                SQL.Text :=  format('SELECT M.MEMBER,%s from MEMBERS M,' +
                                   'M_SUBS MS WHERE M.MEMBER = MS.MEMBER and m.DECEASED <> ''Y'' ',[mFields]);
                                if mGroups <> ''  then SQL.Add( Format( ' AND MS.SUBSCRIPTION in ( %s )',[mGroups]));
                                if FMembers <> '' then SQL.Add( Format( ' %s',[FMembers]));
                                SQL.Add( ' ORDER by M.MEMBER');
                                open;
                             end;
                          end;
                   end;       
                end;         
              4 : begin   // Dates Letter 
                     if YearTo = 0 then YearTo := YearFrom;
                     with dm.qryReport do begin
                        close;
                        SQL.Text := format('SELECT M.MEMBER,M.DATE_OF_BIRTH,%s from members m where m.DECEASED <> ''Y'' ' +
                                           'and (Current_date - M.DATE_OF_BIRTH) / 365 between %d and %d ',[mFields,YearFrom,YearTo]);
                        if FMembers <> '' then SQL.Add( FMembers );
                        SQL.Add( ' ORDER by M.MEMBER');
                        //AddToClipBoard(SQL.Text);
                        open;
                     end;
                end;                                                                                                                     
              5 : begin   // Rollover Letter from frmMembersROLL 
                     with dm.qryReport do begin
                        close;
                        SQL.Text := format('SELECT mr.KEEP_EXPIRES_CURRENT,mr.MEMBER,mr.MAILING_LIST_CODE,' +
                                    'mr.SURNAME,mr.FIRST_NAME,m.FULL_NAME,m.ADDRESS,m.DISTRICT,' + 
                                    'mr.ADDRESS1,mr.CITY,mr.COUNTRY,mr.HOME_PHONE,mr.EXPIRES,' +
                                    'mr.SUB_PAID,mr.IS_ACTIVE,mr.NEWSLETTER,mr.ANALYSE,mr.ACCEPT,' +
                                    '%s from M_ROLLOVER mr,MEMBERS m where mr.MEMBER = m.MEMBER and ' +
                                    'mr.KEEP_EXPIRES_CURRENT <> ''Y'' and mr.ACCEPT = ''Y''',[mFields]);
                        open;
                     end;
                end;                                                                                                                     
              6 : begin   // Rollover Letter from frmMembersROLL 
                     with dm.qryReport do begin
                        close;
                        // each member is in mGroups 
                        SQL.Text := format('SELECT M.MEMBER,%s from MEMBERS M ' +
                                           'where M.MEMBER in ( %s )',[mFields,mGroups]);
                        open;
                     end;
                end;                                                                                                                     
         end; //case
     end;   // with
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     FDateFrom := formatDateTime('mm/dd/yyyy',DateFrom);
     FDateTo   := formatDateTime('mm/dd/yyyy',DateTo);
     with dm.ibdsLetters do begin
         close;
         SELECTSQL.Text := format('SELECT * from Letters WHERE NAME = ''%s''',[Lname]);
         Open;
         FetchAll;
         FSaveLetter := FieldByName('LETTER').AsString; // save 
     end;
     case FWhatBy of
        0,5 : FLine := '{Subs Lines}';     // subs
        //0,5 : FLine := '\{Subs Lines\}';     // subs
        1 : FLine := '\{Sections Lines\}'; // sections
        2,3,4 : FLine := '';
     end;   
     FSections := mGroups; // Sections, Subs, Groups or Events
     result := True;
end;
{ **************************************************************************** }
procedure TfrmDueLetter.DetailBand1BeforePrint(Sender: TQRCustomBand;   var PrintBand: Boolean);
var s : string;
    i,j,k : Longint;
// at this stage only one {Full Name} per line
// if the letter is not 0 or 1 then cant have {Subs Line} or {Sections Lines} in it    
begin                  
     //   \{Full Name\}  = FExportExtFieldsList
     //   FULL_NAME      = FExportFieldsList
     try                                  
       MemberList.Text := FSaveLetter; // gets the whole letter - by line
       FNewLineInsert := 0;
       dm.ibdsLetters.Edit;
       for i := 0 to MemberList.Count -1 do begin
          // check for each field in the line (s)
          for j := 0 to FExportFieldsList.Count -1 do begin 
              k := POS(FExportExtFieldsList[j],MemberList[i]);
              if k > 0 then begin
                 s := MemberList[i];            // get the string
                 MemberList.Delete(i);          // remove from the list
                 System.Delete(s,k,length(FExportExtFieldsList[j])); // remove /{Full Name/}
                 // if the field is null then dont add the line back in 
                 // useful for Address fields
                 if not dm.qryReport.FieldByName(FExportFieldsList[j]).IsNull then begin
                    // if the first digit is numeric then add in a space
                    // it treats the numeric as part of the formatting 
                    if AllDigits(copy(dm.qryReport.FieldByName(FExportFieldsList[j]).AsString,1,1)) then
                       Insert( ' ' + dm.qryReport.FieldByName(FExportFieldsList[j]).AsString,s,k ) // insert FULL_NAME
                    else
                       Insert( dm.qryReport.FieldByName(FExportFieldsList[j]).AsString,s,k ); // insert FULL_NAME
                    MemberList.Insert(i,s);        // insert back into the list - same position
                 end else begin
                    MemberList.Insert(i,'');       // insert blank line
                 end;
              end;
           end;
           if FLine <> '' then begin  // only for Sections/Subs Lines
              k := POS(FLine,MemberList[i]);
              if k > 0 then begin
                 FStartStr := ''; FEndStr   := '';
                 FStartStr := copy(MemberList[i],1,k -1); // copy either side of \{Subs Lines\}
                 FEndStr   := copy(MemberList[i],(k + length(FLine)),Length(MemberList[i]));
                 FNewLineInsert := i;       // only one per letter - cant insert in loop 
                 GetSubString;
              end;
           end;
       end;    
       // insert only for Sections/Subs Lines 
       if FNewLineInsert > 0 then begin
          MemberList.Delete(FNewLineInsert); // remove from the list \{Subs Lines\}
          if MoreThanOneMember then begin // multiple members
             for i := 0 to FNewLineList.count -1 do begin
                MemberList.Insert(FNewLineInsert,FNewLineList[i]);
                inc(FNewLineInsert);
             end;
          end else begin // only one member - dont need name     
             for i := 1 to FNewLineList.count -1 do begin
                MemberList.Insert(FNewLineInsert,FNewLineList[i]);
                inc(FNewLineInsert);
             end;
          end;
       end;
       
       dm.ibdsLetters.FieldByName('LETTER').AsString := MemberList.Text; // assign back
     finally
       //                                   
     end;  
end;
{ **************************************************************************** }
procedure TfrmDueLetter.DetailBand1AfterPrint(Sender: TQRCustomBand;BandPrinted: Boolean);
begin
     //dm.ibdsLetters.cancel; // cancel the edit
end;
{ **************************************************************************** }
procedure TfrmDueLetter.GetSubString;
const
   AddTabs2 = '\tab\tab ';
   AddTabs1 = '\tab ';
var Fee : extended;
    i,j : integer;
    s : string;
    Desc,FirstName,Member : String;
{   Chris
         $30.00    Standard
         $20.00    Junior
    James
         $30.00    Senior
         $20.00    Junior
    Total
         $100.00    
- Does not print names if only one member 
- does not print total if one line
}         
         
begin
     FNewLineList.Clear;
     MoreThanOneMember := False;
     with dm.qryGeneral do begin
        close; // dont get subs with 0.00 fees
        case FWhatBy of
        0 : SQL.Text := format('SELECT M.MEMBER,M.FIRST_NAME,MS.SUBSCRIPTION,S.DESCRIPTION,S.FEE,S.CUST_DISPLAY ' +
                    'FROM MEMBERS M,M_SUBS MS, SUBSCRIPTIONS S WHERE M.MAILING_LIST_CODE = %s ' + 
                    'and M.EXPIRES BETWEEN  ''%s'' AND  ''%s'' and M.MEMBER = MS.MEMBER and MS.SUBSCRIPTION = S.SUBSCRIPTION ' +
                    'and S.FEE <> 0.00 order by ' +
                    'M.MEMBER',[dm.qryReport.FieldByName('MEMBER').AsString,FDateFrom,FDateTo]);
        1 : begin // may be selected sections only
               SQL.Text := format('SELECT M.MEMBER,M.FIRST_NAME,MS.SECTION,S.DESCRIPTION,S.FEE ' +
                    'FROM MEMBERS M,M_SECTIONS MS,SECTIONS S WHERE M.MAILING_LIST_CODE = %s ' + 
                    'and MS.EXPIRES BETWEEN  ''%s'' AND  ''%s'' and M.MEMBER = MS.MEMBER and MS.SECTION = S.SECTION ' +
                    'and S.FEE <> 0.00 ',[dm.qryReport.FieldByName('MEMBER').AsString,FDateFrom,FDateTo]);
               if FSections <> '' then SQL.add(format(' and S.SECTION in ( %s )',[FSections]));       
               SQL.add(' order by M.MEMBER ');
            end; 
        end;
        ExecQuery;
        Fee := 0.00;
        j := 0;
        FirstName := Trim(FieldByName('First_Name').AsString);
        FNewLineList.Add( FStartStr + ' ' + FirstName + FEndStr );
        while not EOF do begin
           inc(j);
           Desc   := ' ' + Trim(FieldByName('DESCRIPTION').AsString);
           Member := ' ' + Trim(FieldByName('Member').AsString);
           if FirstName <> Trim(FieldByName('First_Name').AsString) then begin
              MoreThanOneMember := True;
              FNewLineList.Add( FStartStr + ' ' + Trim(FieldByName('First_Name').AsString) + FEndStr );
           end;   
           FirstName := Trim(FieldByName('First_Name').AsString);
           s := FormatFloat('$###0.00',FieldByName('FEE').AsFloat);
           if length(s) = 5 then  //$1.00
              s := '  ' + s
           else if length(s) = 6 then  //$22.00  
              s := ' ' + s;
           FNewLineList.Add( FStartStr + ' ' +AddTabs2+ s +AddTabs1+ Desc + FEndStr );
           Fee := Fee + FieldByName('FEE').AsFloat; // keep for totals line
           Next;
        end;
        if j > 1 then begin // make a total line if more than one section or subs
           s := FormatFloat('$###0.00',Fee);
           if length(s) = 5 then  //$1.00
              s := '  ' + s
           else if length(s) = 6 then  //$22.00  
              s := ' ' + s;
           FNewLineList.Add( FStartStr + ' Subs Due' + FEndStr );
           FNewLineList.Add( FStartStr + ' ' +AddTabs2+ s + FEndStr);
        end;   
        Close;
     end;
end;
{ **************************************************************************** }
procedure TfrmDueLetter.qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
    // qr.qrPrinter.ongenerateToPrinter := nil;
end;
{ **************************************************************************** }
procedure TfrmDueLetter.FormCreate(Sender: TObject);
begin
    MemberList           := TStringList.Create;
    FExportFieldsList    := TStringList.Create;
    FExportExtFieldsList := TStringList.Create;
    FNewLineList         := TStringList.Create;
    MoreThanOneMember := False;
end;
{ **************************************************************************** }
procedure TfrmDueLetter.FormDestroy(Sender: TObject);
begin
    MemberList.Free;
    FExportFieldsList.Free;
    FExportExtFieldsList.Free;
    FNewLineList.Free;
end;
{ **************************************************************************** }
end.

