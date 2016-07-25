unit rMailingLabels;
{                                                  

 
 NB dont leave spaces at the start of the field else  RemoveBlankLines wont apply - QRExprMemo

 Mailing Label can be created via
    1) Report Selection Criteria
    2) From a previously created report - as long as Member can de seen in qryReport
    
    
 NB mailing labels now use M_ROLLOVER as a base - mailing_code is put into HOME_PHONE 
 This is done because Howick want the Mail Code put onto the labels, anything
 less than 100 does show   
  
}

interface       

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  QuickRpt, Qrctrls, ExtCtrls, IBTable;

type
  TfrmMailingLabels = class(TForm)
    qr: TQuickRep;
    DetailBand: TQRBand;
    memoDetails: TQRRichText;
    function Extract(PrtIndex,Links,MemberOnLabel :integer; idx, mMembers,mNewMembers,mSortLabels,mMailJoin,mMembersEmail :String ) : Boolean;
    function ExtractFromReport( PrtIndex,MemberOnLabel : Integer ) : Boolean;
    procedure FormCreate(Sender: TObject);
    procedure DetailBandBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private                                                        
    { Private declarations }
     FMemberOnLabel : Boolean;
  public
    { Public declarations }
  end;

var
  frmMailingLabels: TfrmMailingLabels;

implementation

{$R *.DFM}

uses
   Global,
   dmMembers;
   
{ **************************************************************************** }   
function TfrmMailingLabels.Extract(PrtIndex,Links,MemberOnLabel :integer; idx,
                                   mMembers,mNewMembers,mSortLabels,mMailJoin,mMembersEmail:String) : Boolean;
var WhereUsed : Boolean;
const SQL1 = 'Delete from M_ROLLOVER';    
      SQL2 = 'INSERT INTO M_ROLLOVER ' +
             'SELECT 0,''N'',m.MEMBER,m.MAILING_LIST_CODE,m.SURNAME,m.KNOWN_AS,m.FULL_NAME,m.ADDRESS,m.DISTRICT,' +
             'm.ADDRESS1,m.CITY,m.COUNTRY,m.MAILING_CODE,CURRENT_DATE,0,''N'',''N'',''N'',''N'' '+
             'from MEMBERS m';
      SQL3 = 'update M_ROLLOVER set HOME_PHONE = null where cast(HOME_PHONE as INTEGER) < 100';
             // put into sub paid a count of Parents and Children 
      SQL4 = 'update m_rollover m set m.sub_paid = (select count(r.mailing_list_code) from m_rollover r ' +
             'where m.mailing_list_code = r.mailing_list_code )';
             // delete where they have a Parent and Child ( not where there ia a child only )
             // eg where group 100 are selected and the Parent is  in group 102 and the child is in 100
      SQL5 = 'delete from m_rollover where sub_paid > 1 and Member <> Mailing_list_code';
begin
     result := False;
     with dm.qryGeneral do begin
         close;
         SQL.Text := SQL1;
         Execquery;
         Close;
         SQL.Text := SQL2;
         //SQL.Text := 'SELECT M.MAILING_LIST_CODE,M.MEMBER,M.FULL_NAME,m.FIRST_NAME,m.SURNAME, ' +
         //            'M.ADDRESS,M.DISTRICT,M.ADDRESS1,M.CITY,M.COUNTRY,MAILING_CODE FROM MEMBERS M';
         case Links of  // add table
            0 : if idx <> '' then SQL.Add( ',M_GROUPS S' );  
            1 : if idx <> '' then SQL.Add( ',M_SECTIONS S' );
            2 : if idx <> '' then SQL.Add( ',M_SUBS S' );
         end;
         WhereUsed := False;
         if mNewMembers <> '' then begin
            SQL.Add( Format( '%s',[mNewMembers]));
            WhereUsed := True;
         end;

         if WhereUsed then 
            SQL.Add( ' and M.DECEASED <> ''Y''' ) // get live ones
         else
            SQL.Add( ' WHERE M.DECEASED <> ''Y''' ); // get live ones
         // this limits the seletion to Parent Code only Members if no Subs , Sections or Events are selected
         if idx = '' then 
            SQL.Add(' and M.MAILING_LIST_CODE = M.MEMBER');
         case Links of  // add table
         0 : begin
             if idx <> '' then begin
                SQL.Add(' and M.MEMBER = S.MEMBER' );
                SQL.Add( Format( ' and S.GROUPS in (%s)',[idx]));
             end;
             end;                   
         1 : begin
             if idx <> '' then begin
                  SQL.Add(' and M.MEMBER = S.MEMBER' );
                  SQL.Add( Format( ' and S.SECTION in (%s)',[idx]));
              end;
              end; 
         2 : begin
             if idx <> '' then begin
                SQL.Add(' and M.MEMBER = S.MEMBER' );
                SQL.Add( Format( ' and S.SUBSCRIPTION in (%s)',[idx]));
              end;
              end;
         end;
         if mMembers      <> '' then SQL.Add( Format( '%s',[mMembers]));
         if mMailJoin     <> '' then SQL.Add( Format( '%s',[mMailJoin]));
         { if the user selects Y or N to Receive Email and they use the Parent
           Code a lot eg Pauanui then the Parent Code Member is the decider
           to wether the family gets a mailing label or not
           eg
           Dad      Y   - doesnt get a label if No is selected
           Mum      N
           Child    N
         }
         if mMembersEmail <> '' then SQL.Add( Format( '%s',[mMembersEmail]));
         AddToClipBoard(SQL.Text);
         Execquery;

         if idx <> '' then begin
            close;
            SQL.Text := SQL4;
            Execquery;
            close;
            SQL.Text := SQL5;
            Execquery;
         end;
         
         
     end;                         
     dm.IBCommitRetain;
     with dm.qryReport do begin
        close;
        { Error Pauanui - because the member is in Multiple Groups,Subs, Sections etc
         then they got duplicate labels        }
        SQL.Text := 'Select Distinct(Member),MAILING_LIST_CODE,SURNAME,FIRST_NAME,FULL_NAME,ADDRESS,ADDRESS1,DISTRICT,CITY,COUNTRY,HOME_PHONE from M_ROLLOVER ' + mSortLabels;
        open;
     end;
     case MemberOnLabel of
        0 : FMemberOnLabel := False;
        1 : FMemberOnLabel := True;
     end;   
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     result := True;
end;
{ **************************************************************************** }
function TfrmMailingLabels.ExtractFromReport( PrtIndex,MemberOnLabel : Integer ) : Boolean;
const SQL1 = 'Delete from lucky_member';
      // lm.Member is actually MAILING_LIST_CODE
      SQL3 = 'Select MAILING_LIST_CODE from MEMBERS where MEMBER = %d';
      SQL2 = 'Select distinct(lm.MEMBER),m.FULL_NAME,m.KNOWN_AS as FIRST_NAME,m.SURNAME,m.ADDRESS,m.ADDRESS1,m.DISTRICT,m.CITY,m.COUNTRY,m.MAILING_CODE ' +
             'from LUCKY_MEMBER lm , MEMBERS m where m.MEMBER = lm.MEMBER and  m.DECEASED <> ''Y'' order by lm.LUCKY_ID';
var tblTemp : TIBTable;
{ MailingListCode is inserted into LUCKY_MEMBER ( from qryReport Member in Order )
  then selected with the mailing details
  qr.DataSet is changed to dm.qryReport1 ( qryReport is already used for the report )
  Done by Mailing List Code to cut down on Mailing Costs
}       
begin
     result := False;
     dm.sqlExecQuery(dm.qryGeneral,SQL1);
     try
        tblTemp := TIBTable.create(nil); 
        tblTemp.Database    := dm.IBDatabase;
        tblTemp.Transaction := dm.IBTransaction;
        tblTemp.Tablename   := 'LUCKY_MEMBER';
        tblTemp.Open;
        dm.qryReport.First;
        while not dm.qryReport.EOF do begin
           // put in the MAILING_LIST_CODE from each member
           if dm.sqlExecQueryCheckEOF(dm.qryGeneral,format(SQL3,[dm.qryReport.FieldByName('MEMBER').AsInteger])) then begin
              tblTemp.Append;
              tblTemp.FieldByName('LUCKY_ID').AsInteger := 0;
              tblTemp.FieldByName('MEMBER').AsInteger := dm.qryGeneral.FieldByName('MAILING_LIST_CODE').AsInteger;
              tblTemp.Post;
           end;
           dm.qryReport.next;
        end;   
        dm.qryOpen(dm.qryReport1 ,SQL2);
        // mrNo and mrYes from MessageDlg 
        case MemberOnLabel of
           7 : FMemberOnLabel := False;
           6 : FMemberOnLabel := True;
        end;   
        result := True;
     finally
        tblTemp.Close;
        tblTemp.Free;
     end;                       
     qr.PrinterSettings.PrinterIndex := PrtIndex;
end;
{ **************************************************************************** }
procedure TfrmMailingLabels.FormCreate(Sender: TObject);
begin 
      with dm.ibdsMailingLabels do begin
         open;
         qr.Page.Columns      := FieldByName('NUMBER_COLUMNS').AsInteger;
         qr.Page.LeftMargin   := FieldByName('LEFT_MARGIN').AsFloat;
         qr.Page.RightMargin  := FieldByName('RIGHT_MARGIN').AsFloat;
         qr.Page.TopMargin    := FieldByName('TOP_MARGIN').AsFloat;
         qr.Page.BottomMargin := FieldByName('BOTTOM_MARGIN').AsFloat;
         qr.Page.ColumnSpace  := FieldByName('COLUMN_SPACE').AsFloat;
         // convert mm to pixels - watch different screen sizes
         DetailBand.Height     := Trunc( FieldByName('BAND_HEIGHT').AsFloat * 3.79 );
         if (FieldByName('FONT_SIZE').IsNull) or (FieldByName('FONT_SIZE').AsInteger = 0) then
            memoDetails.Font.Size := 9
         else   
            memoDetails.Font.Size := FieldByName('FONT_SIZE').AsInteger;
         // now try to adjust memoDetails to bottom left hand corner
         //memoDetails.AlignToBand := True;
        // memoDetails.AlignToBand := False;
        memoDetails.Left := DetailBand.Left;
        case FieldByName('NUMBER_COLUMNS').AsInteger of
            1 : memoDetails.Left := DetailBand.Left + 50;
            2 : memoDetails.Left := DetailBand.Left + 20;
            3 : memoDetails.Left := DetailBand.Left + 10;
            4 : memoDetails.Left := DetailBand.Left + 5;
         end;   
         //memoDetails.Top  := DetailBand.Top  + 5;
         close;
      end;
end;
{ ****************************************************************************}
procedure TfrmMailingLabels.DetailBandBeforePrint(Sender: TQRCustomBand;
  var PrintBand: Boolean);
var i : smallint;
s,MailingCode : string;
begin
     {for i := memoDetails.Lines.Count downto 1 do begin
        s := memoDetails.Lines[i -1];
        if memoDetails.Lines[i -1] = '' then
           memoDetails.Lines.Delete(i -1);
     end;}
     memoDetails.Lines.Clear;
     s := '';
     if CompName = 'Howick Club Inc' then begin
        // KNOWN_AS is put into the first name field
        s := qr.DataSet.FieldByName('FIRST_NAME').AsString + ' ' + qr.DataSet.FieldByName('SURNAME').AsString;
     end else begin
        if qr.DataSet.FieldByName('FULL_NAME').AsString <> '' then 
           s := qr.DataSet.FieldByName('FULL_NAME').AsString;
     end;  
     // sometimes they want the Member Number on the Label
     if FMemberOnLabel then
        memoDetails.Lines.Add('#' + qr.DataSet.FieldByName('MEMBER').AsString + '   ' + s)
     else   
        memoDetails.Lines.Add(s);
     if qr.DataSet.FieldByName('ADDRESS').AsString <> '' then
        memoDetails.Lines.Add(qr.DataSet.FieldByName('ADDRESS').AsString);
     if qr.DataSet.FieldByName('ADDRESS1').AsString <> '' then
        memoDetails.Lines.Add(qr.DataSet.FieldByName('ADDRESS1').AsString);
     if (POS('Papanui',CompName) > 0) then begin
        // dont include Papanui District Field in the mailing labels
     end else begin
        if qr.DataSet.FieldByName('DISTRICT').AsString <> '' then
           memoDetails.Lines.Add(qr.DataSet.FieldByName('DISTRICT').AsString);
     end;      
     if qr.DataSet.FieldByName('CITY').AsString <> '' then
        memoDetails.Lines.Add(qr.DataSet.FieldByName('CITY').AsString);
     if qr.DataSet.FieldByName('COUNTRY').AsString <> '' then
        memoDetails.Lines.Add(qr.DataSet.FieldByName('COUNTRY').AsString);
     MailingCode := '';
     for i := 0 to qr.DataSet.FieldCount - 1 do begin
        if qr.DataSet.Fields[i].FieldName = 'MAILING_CODE' then begin
           MailingCode := qr.DataSet.FieldByName('MAILING_CODE').AsString;
        end;   
        if qr.DataSet.Fields[i].FieldName = 'HOME_PHONE' then begin
           MailingCode := qr.DataSet.FieldByName('HOME_PHONE').AsString;
        end;   
     end;   
     // apply Mail Code to the last line
     if MailingCode <> '' then begin
        s := '';
        i := memoDetails.Lines.Count -1;
        s := memoDetails.Lines[i];
        memoDetails.Lines.Delete(i);
        memoDetails.Lines.add( s + '        ' + MailingCode);
     end;

                                                              
end;

end.








