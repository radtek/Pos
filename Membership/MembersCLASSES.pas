unit MembersCLASSES;

interface

uses            
  Graphics,SysUtils,Windows, Classes,Controls,Dialogs,Messages,Forms,IBQuery,IBTable,DB
  ,dmMembers,dmMenuMate,IBSQL,GLOBAL,IBDatabase,comObj,stdctrls,dbctrls,Wwdbcomb,wwdbedit,Wwdotdot;

type

{ **************************************************************************** }
{ **************************************************************************** }
  // asEdit - DB aware controls  asReport non DB aware controls
  TAsTypes = (asEdit,asReport);              
  { class rules
  1) must have a Heading, Sub Heading and Detail
  2) Check Boxes can be on multiple lines but Text,Yes/No and Numeric 
     must have a unique LINE_NUMBER
}  
  TMemberPreferences = class(TObject)
  private
      fAsTypes : TAsTypes;
      fPrefHeadingLeft    : Smallint;
      fPrefSubHeadingLeft : Smallint;
      fPrefDetailLeft     : Smallint;
      fPrefLineHeight     : smallint;
      fPrefLineTop        : smallint;
      fPrefLabelWidth     : smallint;
      fLabel              : TLabel;
      procedure CreateDetail(var ScrollBox : TScrollBox ; ControlCtr,LineSpacing,ControlCtrPosy : smallint); 
      procedure CreateHeadingLabel(var ScrollBox : TScrollBox;theCaption: ShortString ; theLeftPosy : smallint; DottedLine : Boolean);
      procedure AddCheckBox(var ScrollBox : TScrollBox ;ControlCtrPosy,LineSpacing : smallint);
      procedure AddDBCheckBox(var ScrollBox : TScrollBox ;ControlCtrPosy,LineSpacing : smallint);
      procedure AddYesNoBox(var ScrollBox : TScrollBox  );
      procedure AddTextBox(var ScrollBox : TScrollBox );
   public
    { Public declarations }
     Constructor Create;
     Destructor Destroy;
     procedure MemberPreferenceLoad( var ScrollBox : TScrollBox;AsTypes : TAsTypes );
     
  end;      
{ **************************************************************************** }
{ **************************************************************************** }
{ **************************************************************************** }
{ **************************************************************************** }
// Card Insertion
  TCardMembers = class(TObject)
   public
    { Public declarations }
     function CardInsertMember( theSQL : TIBSQL ; theMember,NumberCards : LongInt ): Boolean;
     procedure CardDeleteMembers( theSQL : TIBSQL );
     procedure AddMemberInCardMembers(Member,NumberCards : LongInt);
     procedure AllFinancialMembers( Paid : Shortint; GroupFields : ShortString );
     
  end;
{ **************************************************************************** }
{ **************************************************************************** }
// Payments
  TMemberPayments = class(TObject)
   public
    { Public declarations }
    procedure PaymentUpdate( theSQL : TIBSQL ; Payment,PaidBy	: Integer;
                          PayAmount,PointsUsed,Discount,Rounding,PayCredit : Extended;
                         EditDAte : TDateTime ;     FullName,InvoiceNumber : shortstring );
     procedure PaymentDetailInsert( theSQL : TIBSQL ;Payment,PaidBy,Member,Code : Integer; 
                   PayFull,PayType : ShortInt;
                   Amount,AmountPaid,Discount : Extended;
                   PayDate : TDateTime ;  Description,Spons,Expires,FullName,InvoiceNumber : shortstring );
     procedure PaymentTenderInsert( theSQL : TIBSQL ; Payment : Integer;
                      TenderType,Details : ShortString; Amount,ExRate : Extended );
     function  GetReceiptNumber ( theSQL : TIBSQL ): LongInt ;
     procedure PaymentUpdateExpires( theExpires,GoodDAte : TDateTime );
     function  PaymentSubPayTypes( Amount,AmountPaid : Extended  ): TSubPayTypes; 
     function PaymentWorkOutExpires( Amount,AmountPaid,Pay : Extended ;
                  SubPayTypes : TSubPayTypes; MExpires,GoodDate : TDateTime; SkipYear,SubsTerm : ShortInt ) : TDateTime;
     function PaymentMemberState( theSQL : TIBSQL ;
                   MExpires : TDAteTime; strMember,SubPayment : shortString; 
                   ReceiptNumber,SubPaid : LongInt ;
                   SubPayTypes: TSubPayTypes): Boolean;

     function  LogPointsCredit( theTransaction : TIBTransaction ; Reason : ShortString ; 
                                BeforeEditPointsCredit : TBeforeEditPointsCredit;
                                DataSet : TDataSet ; PointsValue : Extended  ) : Boolean;
     function PointsCreditUpdate( Value : Extended ; DataSet : TDataSet ) : Boolean ;
  end;                                                       
{ **************************************************************************** }
{ **************************************************************************** }
  TMenuMatePointsCreditTypes = (mmAll,mmPointsEarnt,mmPointsSpent,mmVIP,mmVIPReturn,mmReward,mmRewardReturn);
  
  // MenuMate Points Update from Members_Points table into Members 
  TMenuMatePointsUpdate = class(TObject)
  private
     procedure MenuMatePointsCreditUpdate( Value : Extended ; MMPointsCreditTypes : TMenuMatePointsCreditTypes ;DataSet : TDataset );
  public
     function MembersPointsUpdate( Member : ShortString ) : Boolean;
  end;   
{ **************************************************************************** }
{ **************************************************************************** }
implementation                  
{ **************************************************************************** }
constructor TMemberPreferences.Create;
begin
     fPrefHeadingLeft    := 30;
     fPrefSubHeadingLeft := 45;
     fPrefDetailLeft     := 50;
     fPrefLineHeight     := 30;
     fPrefLineTop        := 5;
     fLabel := TLabel.Create(nil);
     fLabel.AutoSize := True;
end;
{ ****************************************************************************}
destructor TMemberPreferences.Destroy;
begin
    // doesnt get called
     if assigned(fLabel) then
        fLabel.Free;
     sm('destroy');   
end;
{ ****************************************************************************}
procedure TMemberPreferences.MemberPreferenceLoad( var ScrollBox : TScrollBox;AsTypes : TAsTypes );
var ScrollBoxWidth,LineNumber,LineSpacing,ControlCtr,ControlCtrPosy : smallint;
    FirstLine : Boolean;
const SQL1 = 'Select COUNT(HEADING_ID) as ControlCtr from PREF_DETAIL where HEADING_ID = %d and LINE_NUMBER = %d';
begin                      
     fAsTypes := AsTypes;
     // clear the controls
     for ControlCtr := ScrollBox.ComponentCount -1 downto 0 do 
         ScrollBox.Components[ControlCtr].Free;
     ScrollBoxWidth := ScrollBox.Width -  fPrefDetailLeft - ScrollBox.VertScrollBar.Size;
     IBDSActiveOrRefresh (dm.ibdsPrefDetail);
     IBDSActiveOrRefresh (dm.ibdsPrefSubHeading);
     IBDSActiveOrRefresh (dm.ibdsPrefHeading);
     dm.ibdsPrefHeading.First;
     FirstLine := True;
     //dm.ibdsPrefSubHeading.First;
     //dm.ibdsPrefDetail.First;
     // Pref Heading
     while not dm.ibdsPrefHeading.EOF do begin
        if FirstLine then
           FirstLine := False
        else   
           fPrefLineTop   := fPrefLineTop +  fPrefLineHeight;
        CreateHeadingLabel(ScrollBox,dm.ibdsPrefHeading.FieldByName('HEADING_DESC').AsString,fPrefHeadingLeft,False); 
        // Pref Sub Heading
        while not dm.ibdsPrefSubHeading.EOF do begin
           // suppress the printing on Sub Heading labels sometimes
           if dm.ibdsPrefSubHeading.FieldByName('DISPLAY_HEADING').AsString = varYes then begin
              fPrefLineTop   := fPrefLineTop +  fPrefLineHeight;
              CreateHeadingLabel(ScrollBox,dm.ibdsPrefSubHeading.FieldByName('SUB_HEADING_DESC').AsString,fPrefSubHeadingLeft,False); 
           end;   
           LineNumber := dm.ibdsPrefDetail.FieldByName('LINE_NUMBER').AsInteger;
           ControlCtrPosy := 1;
           // count of lines - Check Boxes can have multiple lines 
           // Text,Yes/No and Numeric can only have 1 per line
           dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[dm.ibdsPrefDetail.FieldByName('HEADING_ID').AsInteger,dm.ibdsPrefDetail.FieldByName('LINE_NUMBER').AsInteger]));
           ControlCtr  := dm.qryGeneral.FieldByName('ControlCtr').AsInteger;
           LineSpacing := ScrollBoxWidth div ControlCtr;
           fPrefLineTop   := fPrefLineTop +  fPrefLineHeight;
           // Pref Detail
           while not dm.ibdsPrefDetail.EOF do begin
              fLabel.Caption := trim(dm.ibdsPrefDetail.FieldByName('DETAIL_DESC').AsString);
              fPrefLabelWidth := fLabel.Width;
              // cant have a mix of Check boxs and other controls on the same line 
              if LineNumber = dm.ibdsPrefDetail.FieldByName('LINE_NUMBER').AsInteger then begin
                 // check boxes can only have the same line_number
                 CreateDetail(ScrollBox,ControlCtr,LineSpacing,ControlCtrPosy); 
              end else begin
                 // count of lines - Check Boxes can have multiple lines 
                 // Text,Yes/No and Numeric can only have 1 per line
                 LineNumber     := dm.ibdsPrefDetail.FieldByName('LINE_NUMBER').AsInteger;
                 dm.sqlExecQuery(dm.qryGeneral,format(SQL1,[dm.ibdsPrefDetail.FieldByName('HEADING_ID').AsInteger,dm.ibdsPrefDetail.FieldByName('LINE_NUMBER').AsInteger]));
                 ControlCtr     := dm.qryGeneral.FieldByName('ControlCtr').AsInteger;
                 LineSpacing    := ScrollBoxWidth div ControlCtr;
                 fPrefLineTop   := fPrefLineTop + fPrefLineHeight;
                 ControlCtrPosy := 1;
                 CreateDetail(ScrollBox,ControlCtr,LineSpacing,ControlCtrPosy); 
              end;    
              inc(ControlCtrPosy);
              // if a Check Box - dont alter the position if not printed
              if (dm.ibdsPrefDetail.FieldByName('DETAIL_TYPE').AsString = 'C') and
                (dm.ibdsPrefDetail.FieldByName('DISPLAY_DETAIL').AsString = varNo) then
                 dec(ControlCtrPosy);
                  
              //sm(dm.ibdsPrefDetail.FieldByName('DETAIL_DESC').AsString);
              dm.ibdsPrefDetail.Next;
           end;
           dm.ibdsPrefSubHeading.Next;
        end;  
        dm.ibdsPrefHeading.Next;
     end;
     dm.ibdsPrefHeading.Close;
     dm.ibdsPrefSubHeading.Close;
     dm.ibdsPrefDetail.Close;
     if assigned(fLabel) then
        fLabel.Free;
end;
{ ****************************************************************************}
// Member Preferences
procedure TMemberPreferences.CreateHeadingLabel(var ScrollBox : TScrollBox;theCaption: ShortString ; theLeftPosy : smallint; DottedLine : Boolean);
var aLabel : TLabel;
    BoxLength,DottedLineLength,i : smallint;
begin
     aLabel          := TLabel.Create(ScrollBox);
     aLabel.Parent   := Scrollbox;
     aLabel.Top      := fPrefLineTop;
     aLabel.AutoSize := True;
     aLabel.Left     := theLeftPosy;
     aLabel.Caption  := theCaption;
     // if a Pref Detail label
     if theLeftPosy = fPrefDetailLeft then begin
        if DottedLine then begin
           BoxLength := 50;
           if dm.ibdsPrefDetail.FieldByName('DETAIL_TYPE').AsString = 'T' then
              BoxLength := 150;
           aLabel.AutoSize := False;
           DottedLineLength := ( ScrollBox.Width - ScrollBox.VertScrollBar.Size - BoxLength - 50 );
           DottedLineLength := DottedLineLength -  (fPrefDetailLeft + aLabel.Width);
           for i := 1 to 150 do begin
              aLabel.Caption := aLabel.Caption + ' .'
           end;
           aLabel.Width := alabel.Width + DottedLineLength;
        end;   
     end;   
     // if a Pref Heading label
     if theLeftPosy = fPrefHeadingLeft then begin
        aLabel.Font.Style := [fsBold];
        aLabel.Font.Size  := 9;
     end;
end;                                      
{ ****************************************************************************}
procedure TMemberPreferences.CreateDetail(var ScrollBox : TScrollBox ; ControlCtr,LineSpacing,ControlCtrPosy : smallint); 
begin
     with dm.ibdsPrefDetail do begin
        // type of control
        case fAsTypes of
           asReport : begin
                       if FieldByName('DETAIL_TYPE').AsString = 'C' then begin
                          AddCheckBox(ScrollBox,ControlCtrPosy,LineSpacing);
                       end else begin
                          // label then control
                          CreateHeadingLabel(ScrollBox,dm.ibdsPrefDetail.FieldByName('DETAIL_DESC').AsString,fPrefDetailLeft,True); 
                          if (FieldByName('DETAIL_TYPE').AsString = 'T') or  
                             (FieldByName('DETAIL_TYPE').AsString = 'N') then begin
                             AddTextBox(ScrollBox);
                          end else if FieldByName('DETAIL_TYPE').AsString = 'Y' then begin  
                             AddYesNoBox(ScrollBox);
                          end;   
                       end;   
                    end;   
           asEdit : begin
                       if FieldByName('DETAIL_TYPE').AsString = 'C' then begin
                          AddDBCheckBox(ScrollBox,ControlCtrPosy,LineSpacing);
                       end else begin
                          // label then control
                          CreateHeadingLabel(ScrollBox,dm.ibdsPrefDetail.FieldByName('DETAIL_DESC').AsString,fPrefDetailLeft,True); 
                          //ScrollBoxWidth := ScrollBox.Width -  fPrefDetailLeft - ScrollBox.VertScrollBar.Size;
                          if (FieldByName('DETAIL_TYPE').AsString = 'T') or  
                             (FieldByName('DETAIL_TYPE').AsString = 'N') then begin
                             AddTextBox(ScrollBox);
                          end else if FieldByName('DETAIL_TYPE').AsString = 'Y' then begin  
                             AddYesNoBox(ScrollBox);
                          end;   
                       end;   
                    end;   
        end;
     end;
end;
{ ****************************************************************************}
procedure TMemberPreferences.AddCheckBox(var ScrollBox : TScrollBox ; ControlCtrPosy, LineSpacing: smallint);
var aCheckBox : TCheckBox;         
begin
     // Check Box only
     aCheckBox           := TCheckBox.Create(ScrollBox);
     aCheckBox.Parent    := Scrollbox;
     aCheckBox.AllowGrayed := True;
     aCheckBox.Top       := fPrefLineTop;
     aCheckBox.Caption   := trim(dm.ibdsPrefDetail.FieldByName('DETAIL_DESC').AsString);
     aCheckBox.Width     := fPrefLabelWidth + 25; // allow for box
     aCheckBox.Alignment := taLeftJustify;
     aCheckBox.Left      := ControlCtrPosy * LineSpacing - aCheckBox.Width;
end;
{ ****************************************************************************}
procedure TMemberPreferences.AddDBCheckBox(var ScrollBox : TScrollBox ; ControlCtrPosy, LineSpacing: smallint);
var aCheckBox : TDBCheckBox;         
begin
     // dont display the line 
     if dm.ibdsPrefDetail.FieldByName('DISPLAY_DETAIL').AsString = varNo then exit;
     // DB Check Box only
     aCheckBox            := TDBCheckBox.Create(ScrollBox);
     aCheckBox.Parent     := Scrollbox;
     aCheckBox.Top        := fPrefLineTop;
     aCheckBox.AllowGrayed := True;
     aCheckBox.Caption    := trim(dm.ibdsPrefDetail.FieldByName('DETAIL_DESC').AsString);
     aCheckBox.Width      := fPrefLabelWidth + 25; // allow for box
     aCheckBox.Alignment  := taLeftJustify;
     aCheckBox.Left       := ControlCtrPosy * LineSpacing - aCheckBox.Width;
     aCheckBox.Datasource := dm.dsrPrefMembers;
     aCheckBox.DataField  := dm.ibdsPrefDetail.FieldByName('FIELD_DESC').AsString;
end;
{ ****************************************************************************}
procedure TMemberPreferences.AddTextBox(var ScrollBox : TScrollBox);
var aTextBox : TwwDBEdit;         
begin
     // Text and Numeric only
     // DB and NON DB
     aTextBox            := TwwDBEdit.Create(ScrollBox);
     aTextBox.Parent     := Scrollbox;
     aTextBox.Top        := fPrefLineTop;
     if dm.ibdsPrefDetail.FieldByName('DETAIL_TYPE').AsString = 'T' then
        aTextBox.Width := 150
     else   
        aTextBox.Width := 50;
     aTextBox.Left       := ScrollBox.Width - aTextBox.Width - 50;
     // TwwDBEdit can be bound or unbound from a datasource
     case fAsTypes of
        asEdit : begin
                    aTextBox.Datasource := dm.dsrPrefMembers;
                    aTextBox.DataField  := dm.ibdsPrefDetail.FieldByName('FIELD_DESC').AsString;
                 end;
        asReport : begin
                    if dm.ibdsPrefDetail.FieldByName('DETAIL_DESC').AsString = 'T' then
                       aTextBox.UnboundAlignment :=  taLeftJustify
                    else   
                       aTextBox.UnboundAlignment :=  taRightJustify;
                 end;
     end; // case  
end;
{ **************************************************************************** }
procedure TMemberPreferences.AddYesNoBox(var ScrollBox : TScrollBox );
var aComboBox : TwwDBComboBox;         
begin
     // Text and Numeric only
     aComboBox            := TwwDBComboBox.Create(ScrollBox);
     aComboBox.Parent     := Scrollbox;
     aComboBox.Top        := fPrefLineTop;
     aComboBox.width      := 50;
     //aComboBox.Left       := ControlCtrPosy * LineSpacing - aComboBox.Width;
     aComboBox.Left       := ScrollBox.Width - aComboBox.Width - 50;
     
     // TwwDBEdit can be bound or unbound from a datasource
     case fAsTypes of
        asEdit : begin
                    aComboBox.Datasource := dm.dsrPrefMembers;
                    aComboBox.DataField  := dm.ibdsPrefDetail.FieldByName('FIELD_DESC').AsString;
                    aComboBox.Maplist    := True;
                    aComboBox.Items.Add('Yes' + #9 + 'Y'); 
                    aComboBox.Items.Add('No' + #9 + 'N'); 
                 end;
        asReport : begin
                    if dm.ibdsPrefDetail.FieldByName('DETAIL_DESC').AsString = 'T' then
                       aComboBox.UnboundAlignment :=  taLeftJustify
                    else   
                       aComboBox.UnboundAlignment :=  taRightJustify;
                    aComboBox.Maplist    := False;
                    aComboBox.Items.Add('Yes');
                    aComboBox.Items.Add('No');
                 end;
     end; // case  
end;
{ **************************************************************************** }
{ **************************************************************************** }
{ **************************************************************************** }

// Card Insertion
function TCardMembers.CardInsertMember( theSQL : TIBSQL ; theMember,NumberCards : LongInt ): Boolean;
const SQL1 = 'Insert into CARD_MEMBERS values ( %d, %d,1 )';
      Msg1 = 'Member %d added for a Card';
begin
     try
        Result := False;
        dm.sqlExecQuery(theSQL,format(SQL1,[theMember,NumberCards]));
        //dm.ProcessType := ord(TProcessTypes(ptMembers));
        //dm.LogProcess ( theSQL,format(Msg1,[theMember]) );
        Result := True;
     except
        // do nothing already inserted

     end;   
end;
{ **************************************************************************** }
procedure TCardMembers.CardDeleteMembers( theSQL : TIBSQL );
const SQL1 = 'Delete from CARD_MEMBERS';
begin
     try
        dm.sqlExecQuery(theSQL,SQL1);
     except             
        // do nothing
     end;   
end;
{ **************************************************************************** }
procedure TCardMembers.AddMemberInCardMembers(Member,NumberCards : LongInt);
const msg1 = 'Member %d %s added for a Card';
begin
     dm.StartTrans(dm.IBRunOnce);
     try
        if not CardInsertMember( dm.sqlRunOnce ,Member,NumberCards ) then begin
           dm.IBRunOnce.Rollback;
           sm(format(MemberCardMsg,[Member]));
           exit;
        end;
        dm.IBRunOnce.Commit;
        MdlgI(format(msg1,[Member,'was']));
     except                      
        dm.IBRunOnce.Rollback;
        sm(format(msg1,[Member,'was not']));
     end;
end;
{ **************************************************************************** }
procedure TCardMembers.AllFinancialMembers( Paid : Shortint; GroupFields : ShortString );
const SQL1 = 'INSERT into CARD_MEMBERS SELECT m.MEMBER,1,1 from MEMBERS m %s ';
      SQL2 = ',M_GROUPS mg where m.MEMBER = mg.MEMBER and mg.GROUPS in ( %s )';
var s : string;      
begin
     if not ifmsgdlg('Delete all Members from Manual Card?') then exit;
     dm.StartTrans(dm.IBRunOnce);
     try
        CardDeleteMembers( dm.sqlRunOnce);
        case Paid of 
           0 : dm.sqlExecquery(dm.sqlRunOnce,format(SQL1,['where SUB_PAID > 0']));
           1 : dm.sqlExecquery(dm.sqlRunOnce,format(SQL1,['']));
           2 : begin
                  s := format(SQL2,[GroupFields]);
                  dm.sqlExecquery(dm.sqlRunOnce,format(SQL1,[s])); 
               end;   
        end;   
        dm.IBRunOnce.Commit;
        dm.tblCardMember.Refresh;
        case Paid of 
           0 : MdlgI('All Financial/Part Financial Members Added for a Card');
           1 : MdlgI('All Members Added for a Card');
           2 : MdlgI(Format('All Members in Group/s %s Added for a Card',[GroupFields]));
        end;   
     except                      
        dm.IBRunOnce.Rollback;
        sm('Failed to add Members for a Card');
     end;
end;


{ **************************************************************************** }
//Payments
function TMemberPayments.GetReceiptNumber ( theSQL : TIBSQL ): LongInt ;
var    i : shortint;
const SQL1 = 'INSERT INTO PAYMENTS ( PAYMENT ) VALUES ( %d )';    
      SQL3 = 'SELECT MAX(PAYMENT) as MAX_PAYMENT from PAYMENTS';
begin
     // dont use trigger as it wont roll back
     Result := 0;
     if dm.sqlExecQueryCheckEOF(theSQL,SQL3) then begin
        // first payment
        if theSQL.FieldByName('MAX_PAYMENT').IsNull then 
           Result := 1
        else 
           Result := theSQL.FieldByName('MAX_PAYMENT').AsInteger + 1;
     end else begin
        exit;
     end;   
     // in case somebody has just entered the payment, it will work off the 
     // Primary Key ( PAYMENT ) 
     for i := 1 to 5 do begin
        try
           dm.sqlExecQuery(theSQL,format(SQL1,[Result]));
           break;
        except
           on E : Exception do begin
              if pos(PrimaryKeyMsg,E.Message) > 0 then begin 
                 inc(Result);
                 continue;
              end else begin
                 raise;
              end;   
           end;
        end;
     end;
     if i = 6 then Result := 0;
end;
{ **************************************************************************** }
procedure TMemberPayments.PaymentUpdate( theSQL : TIBSQL ; Payment,PaidBy	: Integer; 
                          PayAmount,PointsUsed,Discount,Rounding,PayCredit : Extended;
                         EditDAte : TDateTime ;     FullName,InvoiceNumber : shortstring );
const SQL1 = 'UPDATE PAYMENTS SET PAY_AMOUNT = %f,POINTS_USED = %f,PAID_BY = %d,' +
             'DISCOUNT = %f,EDIT_DATE = ''%s'',ROUNDING = %f,PAY_CREDIT = %f,FULL_NAME = ' +
             '''%s'',INVOICE_NUMBER = ''%s'' where PAYMENT = %d';    

var LocalEditDate : String[20];
begin
      // see GetReceiptNumber as this Inserts the entry and returns the Number
      LocalEditDate := GetShortDateTime(Editdate);
      // 40 digits only
      if length(FullName) > 40 then
         FullName[0] := chr(40);
      dm.sqlExecQuery(theSQL,format(
       SQL1,[PayAmount,PointsUsed,PaidBy,Discount,LocalEditDAte,Rounding,PayCredit,FullName,InvoiceNumber,Payment]));
end;
{ **************************************************************************** }
procedure TMemberPayments.PaymentDetailInsert( theSQL : TIBSQL ;Payment,PaidBy,Member,Code : Integer;
                   PayFull,PayType : ShortInt;
                   Amount,AmountPaid,Discount : Extended;
                   PayDate : TDateTime ;  Description,Spons,Expires,FullName,InvoiceNumber : shortstring );
const SQL1 =  'INSERT INTO PAYMENTS_DET (PAYMENT,DESCRIPTION,EXPIRES,SPONSORSHIP,' +
              'AMOUNT,AMOUNT_PAID,PAY_FULL,PAY_TYPE,MEMBER,PAY_DATE,FULL_NAME,PAID_BY,' +
              'INVOICE_NUMBER,DISC,CODE) ' +
              'VALUES (%d,''%s'',''%s'',''%s'',%f,%f,%d,%d,%d,''%s'',''%s'',%d,''%s'',%f,%d)';
var LocalPAyDate : String[15];
begin
      LocalPAyDate := GetShortDate(PayDate);
      // no date - use today
      if LocalPAyDate = StrToDate1899 then
        LocalPAyDate := GetShortDate(Date);
      // 25 digits only
      if length(FullName) > 25 then
         FullName[0] := chr(25);
      dm.sqlExecQuery(theSQL,format(
       SQL1,[Payment,Description,Expires,Spons,Amount,AmountPAid,PAyFull,PayType,Member,LocalPAyDate,FullName,PaidBy,InvoiceNumber,Discount,Code]));
end;  
{ **************************************************************************** }
procedure TMemberPayments.PaymentTenderInsert( theSQL : TIBSQL ; Payment : Integer;
                         TenderType,Details : ShortString; Amount,ExRate : Extended );
const SQL1 = 'INSERT INTO PAYMENTS_TEND (PAYMENT,TENDER_TYPE,AMOUNT,DETAILS,EX_RATE) ' +
             'VALUES (%d,''%s'',%f,''%s'',%f)';    
begin        
      dm.sqlExecQuery(theSQL,format(
       SQL1,[Payment,TenderType,Amount,Details,ExRate]));
end;  
{ **************************************************************************** }
procedure TMemberPAyments.PaymentUpdateExpires( theExpires,GoodDate : TDateTime );
// must make sure MExpires is bought up to date if it is behind
// may have missed paying a year or AREV to Delphi ChangeOver
// null or 1899 dates do not get here - weeded out in the select clause
begin
     if Global.FixedExpirySystem then begin
        if theExpires < (Global.MemberExpiry - 365) then
           theExpires := Global.MemberExpiry - 365; 
      end else begin     
        if theExpires < (theExpires - 365) then
           theExpires := AlterExpireDateBelow(theExpires, GoodDate );
     end;
end;
{ **************************************************************************** }                                 
function TMemberPayments.PaymentSubPayTypes( Amount,AmountPaid : Extended ): TSubPayTypes; 
begin
     if AmountPaid >= Amount then
        Result := spFull
     else if AmountPaid = 0.00 then
        Result := spZero
     else Result := spPart;
end;     
{ **************************************************************************** }     
function TMemberPayments.PaymentWorkOutExpires( Amount,AmountPaid,Pay : Extended ;
          SubPayTypes : TSubPayTypes; MExpires,GoodDate : TDateTime; SkipYear,SubsTerm : ShortInt ): TDateTime;
var Yrs : ShortInt;
begin
     case SubPayTypes of
        spZero : begin
           { bring date up to the the latest Year but below GoodDate
            only if they are >= FGoodDate
            eg MExpires = 01/05/1998 GoodDate = 07/07/2000 then Result = 01/05/2000
            or MExpires = 11/08/2005 GoodDate = 07/07/2000 then Result = 11/08/1999 }
            if MExpires >= GoodDate then
               Result := Global.AlterExpireDateBelow(MExpires,GoodDate);
            end;
        spPart : begin
              { Part Paid - good up until next anniversary  either by credit or payment
                do this because the next time they pay - the part value is
                not cleared  - they will be made un-financial on rollover}
               // part paid but they are ahead- just before they expire
               if Global.FixedExpirySystem and (MExpires > Global.MemberExpiry) then 
                  Result := MExpires
               else if Global.FixedExpirySystem and (MExpires = Global.MemberExpiry) then 
                  Result := incMonth(MExpires,(SubsTerm))
               else
                  Result := Global.AlterExpireDateAbove(MExpires,GoodDate);
               { no POS - do a rollover before expires maybe
                 therefore their Expires could be ahead and they are spPart
                 check to see if they are up to the rolled over expires date }
               if global.FixedExpirySystem and (Result < global.MemberExpiry) then
                  Result := global.MemberExpiry;
                  end;
         spFull : begin
            { if they have paid exactly the same amount as last year
              they are presumed to have paid for next year prior to expires
              watch if the subs go up it may not work }
            if ((Pay * 2) = AmountPaid) and (MExpires > GoodDate) then begin
               //Result := Global.AddYears(MExpires,1);

               Result := incMonth(MExpires,SubsTerm);
               
            // if pay = amount then add 1 year
            end else if Pay = Amount then begin
               // New Member and 30-60 days before Expires - setting in MISC }
               //Result := Global.AddYears(MExpires,(1 + SkipYear));
               Result := incMonth(MExpires,(SubsTerm + SkipYear));

            // if pay > amount then add ? years added 28/06/04 for Upper Hutt
            end else if Pay > Amount then begin
               // New Member and 30-60 days before Expires - setting in MISC }
               Result := incMonth(MExpires,((trunc(Pay / Amount) * SubsTerm) + SkipYear));
               
            // addition of a sub thru the year - the expires stays the same - not a credit
            end else if (Pay < AmountPaid) and (MExpires > GoodDate) and (Pay > 0.00 ) then begin
               // do nothing - even if a skipyear 
               Result := MExpires;
            
            end else begin
               { PrePaid could be years ahead and a credit or payment
                need to add or subtract years - NB could be 2 Subs and one of them gets a payment
                but they are already 2 years ahead - it will only
                add a year if the Pay / Amount = 1 more year.
                Even if it was credit payment - it is at least 1 year up }
               if Pay > 0.00 then begin
                  // not passed expires
                  if GoodDate < MExpires then begin
                      // Upper Hutt change 29/04/04
                     // MExpires := Global.AlterExpireDateBelow(MExpires,GoodDate);
                     //Yrs := trunc (AmountPaid / Amount );
                     Yrs := trunc (Pay / Amount );
                     //Result := Global.AddYears(MExpires,(Yrs + SkipYear));
                     //  Term Payment * SubsTerm
                     SubsTerm := SubsTerm * Yrs;
                     Result := incMonth(MExpires,(SubsTerm + SkipYear));
                  // passed expires - AmountPaid should be 0.00 upon entry
                  end else begin   
                     Yrs := trunc (Pay / Amount );
                     //Result := Global.AddYears(MExpires,(Yrs + SkipYear));
                     SubsTerm := SubsTerm * Yrs;
                     Result := incMonth(MExpires,(SubsTerm + SkipYear));
                  end; 
               end else if Pay < 0.00 then begin // credit
                  Pay := abs(Pay);
                  if Pay > Amount then begin
                     Yrs := trunc ( Pay / Amount );
                     Yrs := Yrs - (2 * Yrs);   //turn negative
                     Result := Global.AddYears(MExpires,Yrs); 
                  end else begin
                     Result := Global.AddYears(MExpires,-1);// take at least one off 
                  end;   
               end;
            end;
        end;
     end;
end;
{ **************************************************************************** }
function TMemberPayments.PaymentMemberState( theSQL : TIBSQL ;
                   MExpires : TDAteTime; strMember,SubPayment : shortString;
                   ReceiptNumber,SubPaid : LongInt ;
                   SubPayTypes : TSubPayTypes ): Boolean;
const SQL3 = 'UPDATE PAYMENTS_DET SET EXPIRES = ''%s'' where MEMBER = %s and PAYMENT = %d ' +
             ' and PAY_TYPE in (%d,%d)'; // Subs or Join Fee - not Sections
      {SQL4 = 'UPDATE MEMBERS SET SUB_PAID = %d,EDIT_DATE = ''%s'',EXPIRES = ''%s'',IS_ACTIVE = ''Y'',' +
             'INACTIVE_DATE = null,SUB_PAYMENT = ''Y'' where MEMBER = %s'; 
      SQL5 = 'UPDATE MEMBERS SET SUB_PAID = %d,EDIT_DATE = ''%s'',EXPIRES = ''%s'', ' +
             'SUB_PAYMENT = ''Y'' where MEMBER = %s';}
      SQL4 = 'UPDATE MEMBERS SET SUB_PAID = %d,EXPIRES = ''%s'',IS_ACTIVE = ''%s'',SUB_PAYMENT = ''Y'',';
      SQL6 = ' INACTIVE_DATE = current_date where MEMBER = %s';
      SQL5 = ' INACTIVE_DATE = null where MEMBER = %s'; 
begin
     Result := False;
     // Set Payments_Det EXPIRES to MExpires 
     dm.sqlExecQuery(theSQL,format(SQL3,[GetShortDate(MExpires),strMember,ReceiptNumber,ord(TPayTypes(ptSubs)),ord(TPayTypes(ptJoinFee))]));
     {if SubPayTypes = spFull then begin
        // make IS_ACTIVE to 'T' straight away
        dm.sqlExecQuery(theSQL,format(SQL4,[ord(SubPayTypes),GetShortDateTime(now),GetShortDate(MExpires),strMember]));
        // Log export because Member is becoming Financial and Active
        if SubPaid <> ord(spFull) then
           if not dm.LogExport(theSQL,StrToInt(strMember),0) then exit;
     end else begin
        // else let the rollover handle the IS_ACTIVE
        // Between Expires and Fixed Rollover date or Revolving x days after
        // they can remain as an Active Member
        dm.sqlExecQuery(dm.sqlTransPay,format(SQL5,[ord(SubPayTypes),GetShortDateTime(now),GetShortDate(MExpires),strMember]));
     end; }
     case SubPayTypes of
        spFull,
        spPart : begin
                    // Upper Hutt New Member 1st Time Paid - dont make them Active or Financial
                    if FirstPaymentNonActive and (SubPayment = varNo) then begin
                       SubPayTypes := spZero;
                       dm.sqlExecQuery(dm.sqlTransPay,format(SQL4 + SQL6,[ord(SubPayTypes),GetShortDate(MExpires),'N',strMember]));
                    end else begin
                       dm.sqlExecQuery(dm.sqlTransPay,format(SQL4 + SQL5,[ord(SubPayTypes),GetShortDate(MExpires),'Y',strMember]));
                    end;
                 end;
        spZero : dm.sqlExecQuery(dm.sqlTransPay,format(SQL4 + SQL6,[ord(SubPayTypes),GetShortDate(MExpires),'N',strMember]));
     end;
     // log all changes
     if not dm.LogExport(dm.sqlTransPay,StrToInt(strMember),0,False) then begin
        Result := False;
        exit;
     end;
     Result := true;
end;
{ **************************************************************************** }
function  TMemberPayments.LogPointsCredit( theTransaction : TIBTransaction ; 
                                           Reason : ShortString ;
                                           BeforeEditPointsCredit : TBeforeEditPointsCredit;
                                           DataSet : TDataSet; PointsValue : Extended ) : Boolean;
begin
     Result := False;
     try
        with dm.ibdsPointsLog do begin
           Close;
           Transaction := theTransaction;
           Open;
           Insert;
           with BeforeEditPointsCredit do begin
              FieldByName('BEF_TOTAL_EARNT').AsFloat     := beTotalEarnt;
              FieldByName('BEF_TOTAL_REDEEMED').AsFloat  := beTotalRedeemed;
              FieldByName('BEF_EARNT_CREDIT').AsFloat    := beEarntCredit;
              FieldByName('BEF_BONUS_CREDIT').AsFloat    := beBonusCredit;
              FieldByName('BEF_PRE_PAID_CREDIT').AsFloat := bePrePaidCredit;
              FieldByName('BEF_POINTS_AVAILABLE').AsFloat := bePointsAvailable;
           end;
           FieldByName('POINTS_ID').AsInteger     := 0;
           FieldByName('TOTAL_EARNT').AsFloat     :=  DataSet.FieldByName('TOTAL_EARNT').AsFloat;
           FieldByName('TOTAL_REDEEMED').AsFloat  :=  DataSet.FieldByName('TOTAL_REDEEMED').AsFloat;
           FieldByName('EARNT_CREDIT').AsFloat    :=  DataSet.FieldByName('EARNT_CREDIT').AsFloat;
           FieldByName('BONUS_CREDIT').AsFloat    :=  DataSet.FieldByName('BONUS_CREDIT').AsFloat;
           FieldByName('PRE_PAID_CREDIT').AsFloat :=  DataSet.FieldByName('PRE_PAID_CREDIT').AsFloat;
           FieldByName('POINTS_AVAILABLE').AsFloat :=  DataSet.FieldByName('POINTS_AVAILABLE').AsFloat;
           FieldByName('MEMBER').AsInteger        :=  DataSet.FieldByName('MEMBER').AsInteger;
           FieldByName('USER_CODE').AsInteger     :=  UserCode;
           FieldByName('REASON').AsString         :=  Reason;
           FieldByName('POINTS_VALUE').AsFloat    :=  PointsValue;
           FieldByName('EDIT_DATE').AsDateTime    :=  now;
           FieldByName('PROCESS').AsInteger       :=  ord(PointsCreditTypes);
           FieldByName('FULL_NAME').AsString      :=  DataSet.FieldByName('FULL_NAME').AsString;
           Post;
        end;                           
        Result := True;
     except
        sm(format('Points Log for Member %d has failed',[DataSet.FieldByName('MEMBER').AsInteger]));
        Raise;
     end;
end;
{ **************************************************************************** }
function TMemberPayments.PointsCreditUpdate( Value : Extended ; DataSet : TDataSet ) : Boolean ;
{
pcAll,pcMaintenance - not done here
*pcSetup         +/- Value  TOTAL_EARNT ,BONUS_CREDIT                    Settings 
*pcSubPaidOffice -Value  TOTAL_REDEEMED                                  Payments
*pcCredit        +Value  TOTAL_EARNT, PRE_PAID_CREDIT                    POS VIP Credit
*pcLucky         +Value  TOTAL_EARNT, BONUS_CREDIT                       POS lucky Draw
*pcSubPaidPOS    -Value  TOTAL_REDEEMED                                  POS Subs Paid
*pcPointsEarnt   +Value  TOTAL_EARNT ,EARNT_CREDIT                       POS Points Earnt
*pcPointsSpent   -Value  TOTAL_REDEEMED                                  POS Points Spent
*pcReset                 All Points/Credit                 Settings and Edit- new member
}
begin
     // NB DataSet is in EDIT mode
     Result := False;
     with DataSet do begin
        case PointsCreditTypes of 
           pcSetup,pcLucky : 
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat  + Value;
               FieldByName('TOTAL_EARNT').AsFloat       := FieldByName('TOTAL_EARNT').AsFloat  + Value;
               FieldByName('BONUS_CREDIT').AsFloat      := FieldByName('BONUS_CREDIT').AsFloat + Value;
             end;
           pcPointsEarnt : 
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat  + Value;
               FieldByName('TOTAL_EARNT').AsFloat       := FieldByName('TOTAL_EARNT').AsFloat + Value;
               FieldByName('EARNT_CREDIT').AsFloat      := FieldByName('EARNT_CREDIT').AsFloat + Value;
               FieldByName('CARD_LAST_USED').AsDateTime := Date;
             end;                          
           pcCredit :   
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat  + Value;
               FieldByName('TOTAL_EARNT').AsFloat       := FieldByName('TOTAL_EARNT').AsFloat + Value;
               FieldByName('PRE_PAID_CREDIT').AsFloat   := FieldByName('PRE_PAID_CREDIT').AsFloat + Value;
               FieldByName('CARD_LAST_USED').AsDateTime := Date;
             end;
           pcCardLastUsed :
             begin
               FieldByName('CARD_LAST_USED').AsDateTime := Date;
             end;          
           pcSubPaidPOS,pcPointsSpent:   
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat - Value;
               FieldByName('TOTAL_REDEEMED').AsFloat    := FieldByName('TOTAL_REDEEMED').AsFloat + Value;
               FieldByName('CARD_LAST_USED').AsDateTime := Date;
             end;          
           pcSubPaidOffice :   
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat - Value;
               FieldByName('TOTAL_REDEEMED').AsFloat    := FieldByName('TOTAL_REDEEMED').AsFloat + Value;
             end;                          
           pcReset:
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := 0.00;
               FieldByName('TOTAL_REDEEMED').AsFloat    := 0.00;
               FieldByName('TOTAL_EARNT').AsFloat       := 0.00;
               FieldByName('PRE_PAID_CREDIT').AsFloat   := 0.00;
               FieldByName('BONUS_CREDIT').AsFloat      := 0.00;
               FieldByName('EARNT_CREDIT').AsFloat      := 0.00;
             end;
        end;
     end;   
     Result := True;
     
end;
{ **************************************************************************** }
{ **************************************************************************** }
{ **************************************************************************** }
// MenuMate Points Update from Members_Points table into Members 
function TMenuMatePointsUpdate.MembersPointsUpdate ( Member : ShortString ) : Boolean;
var Earned,Redeemed : Boolean;
    MenuMatePointsCreditTypes : TMenuMatePointsCreditTypes;
    DonationPointsEarned,PointsEarned : Extended;
const SQL_ALL =    'select MEMBERS_POINTS_KEY,ADJUSTMENT_TYPE, EARNED,MEMBER,' +
                   'POINTS_TYPE, REDEEMED from MEMBERS_POINTS';
      SQL_MAX     = ' where MEMBERS_POINTS_KEY <= %d'; 
      SQL_SINGLE  = ' where MEMBER in (%s)'; 
begin
{

MenuMatePointsCreditTypes = (mmAll,mmPointsEarnt,mmPointsSpent,mmVIP,mmVIPReturn,mmReward, mmRewardReturn);

ADJUSTMENT_TYPE INTEGER
This is the type of Adjustment that has been performed.
0)      Manual Adjustment from Membership.
1)      Stock Master Adjustment from Membership.
2)      Payment Adjustment from Membership.
3)  *   Manual Adjustment from MenuMate.
4)  *   Adjustment from MenuMate.
5)  *   Birthday Adjustment from MenuMate.

POINTS_TYPE INTEGER,
0)      Normal earned points.
1)      VIP Purchased Points.
2)      Reward Points. 
}
    // MenuMate only Function - make sure it is set for MenuMate Sites
    if not MenuMateInstalled then begin
       Result := true;
       exit;
    end;
    screen.Cursor := crHourGlass;
    try
       with dmMM do begin
       Result := false;
       if not IBMenuMatePoints.InTransaction then
          IBMenuMatePoints.StartTransaction;
       try
          // determine wether its one Member or all Members 
          if Member = '-1' then begin
             sqlMemberPointsKey.ExecQuery;
             // get the Max then select on <= max to only get the current range of Members
             // not ones that are added on as you process these records
             ibdsMembersPoints.SelectSQL.Text := SQL_ALL + format(SQL_MAX,[sqlMemberPointsKey.FieldByName('MaxMemberPointsKey').AsInteger]);
          end else begin
             // sometimes passed as a string of Members 1,2,3
             ibdsMembersPoints.SelectSQL.Text := SQL_ALL + format(SQL_SINGLE,[Member]);
             //ibdsMembersPoints.ParamByName('MEMBER').AsString := Member;
          end;   
          // reads Members Points
          ibdsMembersPoints.Open;   
          // no records to process
          if ibdsMembersPoints.EOF then begin
             ibdsMembersPoints.Close;   
             if IBMenuMatePoints.InTransaction then
                IBMenuMatePoints.Commit;
             exit;   
          end;
          ibdsMembersPoints.First;   
          while not ibdsMembersPoints.EOF do begin
             ibdsUpdateMemberPoints.Close;
             ibdsUpdateMemberPoints.ParamByName('MEMBER').AsInteger := ibdsMembersPoints.FieldByName('MEMBER').AsInteger;
             ibdsUpdateMemberPoints.Open;
             // no Member in the file - just continue at this stage - they will stay in the log file 
             if ibdsUpdateMemberPoints.EOF then begin
                ibdsMembersPoints.Next;
                continue;
             end;
             Earned := false; Redeemed := false;
             if ibdsMembersPoints.FieldByName('EARNED').AsFloat <> 0.00   then Earned   := true;
             if ibdsMembersPoints.FieldByName('REDEEMED').AsFloat <> 0.00 then Redeemed := true;
             // Dont do split here - do it by the trigger
             // Donation Member - only allow on Manual Adj or Adj from MenuMate with a Points Type of normal
             // Only if Earned, Donation Member <> 0 and Donation > 0 
             {if (ibdsMembersPoints.FieldByName('ADJUSTMENT_TYPE').AsInteger in [3,4] ) and  
                (ibdsMembersPoints.FieldByName('POINTS_TYPE').AsInteger = 0) and
                (ibdsUpdateMemberPoints.FieldByName('DONATION_MEMBER').AsInteger <> 0) and 
                (ibdsUpdateMemberPoints.FieldByName('DONATION').AsInteger > 0) and Earned then begin
                // open with the Donation Member
                ibdsUpdateDonationMember.Close;
                ibdsUpdateDonationMember.ParamByName('MEMBER').AsInteger := ibdsUpdateMemberPoints.FieldByName('DONATION_MEMBER').AsInteger;
                ibdsUpdateDonationMember.Open;
                // Give the Donation Member their cut of the points
                if not ibdsUpdateDonationMember.EOF then begin
                   DonationPointsEarned := (ibdsMembersPoints.FieldByName('EARNED').AsFloat * ibdsUpdateMemberPoints.FieldByName('DONATION').AsFloat ) / 100;
                   PointsEarned         := ibdsMembersPoints.FieldByName('EARNED').AsFloat  - DonationPointsEarned;
                   ibdsUpdateDonationMember.Edit;
                   MenuMatePointsCreditUpdate(DonationPointsEarned,mmPointsEarnt,ibdsUpdateDonationMember);
                   ibdsUpdateDonationMember.Post;
                // There is no Donation Member - give it back to the original Member
                end else begin
                   PointsEarned := ibdsMembersPoints.FieldByName('EARNED').AsFloat;
                end;
                ibdsUpdateDonationMember.Close;
             end else begin
                PointsEarned := ibdsMembersPoints.FieldByName('EARNED').AsFloat;
             end;}     
             PointsEarned := ibdsMembersPoints.FieldByName('EARNED').AsFloat;
             // put into edit mode for MenuMatePointsCreditUpdate
             ibdsUpdateMemberPoints.Edit;
             case ibdsMembersPoints.FieldByName('ADJUSTMENT_TYPE').AsInteger of 
                3,4 : begin // Manual Adjustment
                       case ibdsMembersPoints.FieldByName('POINTS_TYPE').AsInteger of 
                          0 : begin  // Normal Earned Points
                                 // Adj Type of 3 or 4 and Points Type of 0 are allowed for Donating Points 
                                 if Earned then   MenuMatePointsCreditUpdate(PointsEarned,mmPointsEarnt,ibdsUpdateMemberPoints);
                                 if Redeemed then MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('REDEEMED').AsFloat,mmPointsSpent,ibdsUpdateMemberPoints);
                              end;
                          1 : begin  // VIP Purchased Points
                                 if Earned then   MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('EARNED').AsFloat,mmVIP,ibdsUpdateMemberPoints);
                                 if Redeemed then MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('REDEEMED').AsFloat,mmVIPReturn,ibdsUpdateMemberPoints);
                              end;
                          2 : begin  // Reward Points
                                 if Earned then   MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('EARNED').AsFloat,mmReward,ibdsUpdateMemberPoints);
                                 if Redeemed then MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('REDEEMED').AsFloat,mmRewardReturn,ibdsUpdateMemberPoints);
                              end;          
                       else
                          // no POINTS_TYPE - they will stay in the log file and wont get lost 
                          ibdsMembersPoints.Next;
                          continue;
                       end;       
                    end;
                5 : begin // Birthday Adjustment
                       case ibdsMembersPoints.FieldByName('POINTS_TYPE').AsInteger of 
                          1 : begin  // VIP Purchased Points
                                 if Earned then   MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('EARNED').AsFloat,mmVIP,ibdsUpdateMemberPoints);
                                 if Redeemed then MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('REDEEMED').AsFloat,mmVIPReturn,ibdsUpdateMemberPoints);
                              end;
                          0,2 : begin  // Normal Earned Points & Reward Points
                                 if Earned then   MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('EARNED').AsFloat,mmReward,ibdsUpdateMemberPoints);
                                 if Redeemed then MenuMatePointsCreditUpdate(ibdsMembersPoints.FieldByName('REDEEMED').AsFloat,mmRewardReturn,ibdsUpdateMemberPoints);
                              end;   
                       else
                          // no POINTS_TYPE - they will stay in the log file and wont get lost 
                          ibdsMembersPoints.Next;
                          continue;      
                       end;       
                   end;
             else
                //  no ADJUSTMENT_TYPE - they will stay in the log file and wont get lost
                ibdsMembersPoints.Next;
                continue; 
             end; // end case
             if ibdsUpdateMemberPoints.State = dsEdit then
                ibdsUpdateMemberPoints.Post;
             // now delete each record from the Member_Points file once it has been processed 
            // showmessage(ibdsMembersPoints.FieldByName('members_points_key').AsString);
             ibdsMembersPoints.Delete;
          end;   
          ibdsMembersPoints.Close;
          //showmessage('Commit');   
          // commit the transaction
          if IBMenuMatePoints.InTransaction then begin
             IBMenuMatePoints.Commit;
          end;   
          Result := true;
       except
          //On E : Exception do begin
            //  if Pos( LockConflictMsg ,E.Message) > 0 then begin 
            //     sm('2 PCs are updating the points' );
            //  end;
             // Rollback
             if IBMenuMatePoints.InTransaction then begin
                IBMenuMatePoints.Rollback;
             end;   
          //end;
       end;   
       end; // lazy with   
   finally
      screen.Cursor := crDefault;
   end;    
end;
{ ****************************************************************************}
procedure TMenuMatePointsUpdate.MenuMatePointsCreditUpdate( Value : Extended ; MMPointsCreditTypes : TMenuMatePointsCreditTypes; DataSet : TDataset );
begin
   {
   ADJUSTMENT_TYPE INTEGER
   3)  *   Manual Adjustment from MenuMate.
   4)  *   Adjustment from MenuMate.
   5)  *   Birthday Adjustment from MenuMate.
   POINTS_TYPE INTEGER,
   0)      Normal earned points.
   1)      VIP Purchased Points.
   2)      Reward Points. 

   MenuMatePointsCreditTypes = (mmAll,mmPointsEarnt,mmPointsSpent,mmVIP,mmVIPReturn,mmReward,mmRewardReturn);
   }
     with DataSet do begin
        //showmessage(FieldByName('MEMBER').AsString);
        FieldByName('CARD_LAST_USED').AsDateTime := Date;
        case MMPointsCreditTypes of 
           mmPointsEarnt : 
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat  + Value;
               FieldByName('TOTAL_EARNT').AsFloat       := FieldByName('TOTAL_EARNT').AsFloat       + Value;
               FieldByName('EARNT_CREDIT').AsFloat      := FieldByName('EARNT_CREDIT').AsFloat      + Value;
             end;                          
           mmPointsSpent:   
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat - Value;
               FieldByName('TOTAL_REDEEMED').AsFloat    := FieldByName('TOTAL_REDEEMED').AsFloat   + Value;
             end;          
           mmVIP :   
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat  + Value;
               FieldByName('TOTAL_EARNT').AsFloat       := FieldByName('TOTAL_EARNT').AsFloat       + Value;
               FieldByName('PRE_PAID_CREDIT').AsFloat   := FieldByName('PRE_PAID_CREDIT').AsFloat   + Value;
             end;
           mmVIPReturn :   
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat  - Value;
               FieldByName('TOTAL_REDEEMED').AsFloat    := FieldByName('TOTAL_REDEEMED').AsFloat    + Value;
               FieldByName('PRE_PAID_CREDIT').AsFloat   := FieldByName('PRE_PAID_CREDIT').AsFloat   - Value;
             end;
           mmReward : 
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat  + Value;
               FieldByName('TOTAL_EARNT').AsFloat       := FieldByName('TOTAL_EARNT').AsFloat       + Value;
               FieldByName('BONUS_CREDIT').AsFloat      := FieldByName('BONUS_CREDIT').AsFloat      + Value;
             end;
           mmRewardReturn : 
             begin
               FieldByName('POINTS_AVAILABLE').AsFloat  := FieldByName('POINTS_AVAILABLE').AsFloat  - Value;
               FieldByName('TOTAL_REDEEMED').AsFloat    := FieldByName('TOTAL_REDEEMED').AsFloat    + Value;
               FieldByName('BONUS_CREDIT').AsFloat      := FieldByName('BONUS_CREDIT').AsFloat      - Value;
             end;
         end; // end case
     end;   
end;
{ **************************************************************************** }


end.
