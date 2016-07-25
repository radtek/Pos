unit Tenders;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  stdForm, ComCtrls, Buttons, StdCtrls, ExtCtrls, Grids,
  Wwdbigrd, Wwdbgrid,DB, Mask, wwdbedit, lmdcompo, lmdclass, lmdnonvS,
  Menus, ToolWin;

type
  TfrmTenders = class(TSMStdForm)
    dbgTenders: TwwDBGrid;
    menuExit: TMenuItem;
    pnlSpace: TPanel;
    Panel3: TPanel;
    btnOK: TToolButton;
    ToolButton2: TToolButton;
    Panel1: TPanel;
    StatusBar1: TStatusBar;
    dbePayment: TSpeedButton;
    dbeChange: TSpeedButton;
    procedure FormActivate(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDeactivate(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure dbgTendersFieldChanged(Sender: TObject; Field: TField);
    procedure SetFields;
    procedure dbgTendersRowChanged(Sender: TObject);
    procedure dbgTendersDblClick(Sender: TObject);
    procedure menuExitClick(Sender: TObject);
    procedure FormKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure dbgTendersCalcCellColors(Sender: TObject; Field: TField;
      State: TGridDrawState; Highlight: Boolean; AFont: TFont;
      ABrush: TBrush);
    procedure FormResize(Sender: TObject);

  private
    { Private declarations }
    FTotalAmount : extended;
    FAmount     : string[10];
    FDetails    : string[30];
    FTenderType : string[30];
    FKeepName   : string[30];
    FRoundValue : Extended;
    FInProcess  : Boolean;
    FChangeAmount  : Boolean;
    FChangeDetails : Boolean;
    FCash          : Boolean;
    FpnlWidth      : smallint;
  public
    { Public declarations }
    CashAmount : extended;
    RndValue : extended;
    class function ScreenCode : Integer; override;
    class function ScreenName : String; override;
    class function ScreenDesc : String; override;

  end;

var
  frmTenders: TfrmTenders;

implementation

{$R *.DFM}
uses
     Global, dmMembers;

type  TRoundValue = array[0..9] of Real;
// swedish rounding
const RoundValue : TRoundValue = ( 0.00,-0.01,-0.02,0.02,0.01,0.00,-0.01,-0.02,0.02,0.01);

{ **************************************************************************** }
class function TfrmTenders.ScreenCode : Integer;
begin Result := Global.scTenderPayments; end;
{ **************************************************************************** }
class function TfrmTenders.ScreenName : String;
begin Result := 'Tender Payments       '; end;
{ **************************************************************************** }
class function TfrmTenders.ScreenDesc : String;
begin Result := '  Tender Payments Screen'; end;
{ **************************************************************************** }
procedure TfrmTenders.FormActivate(Sender: TObject);
begin
     inherited;
     dm.ibdsTenders.Open; // get the latest tenders
     dm.ibdsTenders.First;
     // set up FCash if the first record is sitting on cash // very likely
     FTenderType := dm.ibdsTenders.FieldByName('TENDER_TYPE').AsString;
     FKeepName   := dm.ibdsTenders.FieldByName('TENDER_TYPE').AsString;
     if (FTenderType = 'Cash') or (FTenderType = 'CASH') or (FTenderType = 'cash') then
        FCash := True
     else
        FCash := False;
     dbePayment.Caption := floatToStrf(CashAmount,ffCurrency,7,2);
     dbeChange.Caption := '$0.00';
     FDetails := '';
     dbgTenders.SetFocus;
     dbgTenders.SetActiveField('AMOUNT');
end;
{ **************************************************************************** }
procedure TfrmTenders.FormCreate(Sender: TObject);
begin
     inherited;
     FInProcess := False;
     FpnlWidth  := pnlSpace.Width;
end;
{ **************************************************************************** }
procedure TfrmTenders.FormDeactivate(Sender: TObject);
begin
     inherited;
end;
{ **************************************************************************** }
procedure TfrmTenders.btnOKClick(Sender: TObject);
begin
     if dbePayment.Caption <> '$0.00' then begin // not all reduced
        ModalResult := mrNone;
        exit;
     end else 
       ModalResult := mrOK;
end;
{ **************************************************************************** }
procedure TfrmTenders.dbgTendersFieldChanged(Sender: TObject;Field: TField);
// Cannot process while doing Proc SetFields - FTenderType is captured in wwDBGrid1RowChanged
begin
    If FInProcess then exit;
    FKeepName := (Sender as TwwDBGrid).Fields[0].DisplayText;
    if FTenderType = '' then begin
       FDetails := '';
       FAmount  := '';
       FChangeAmount  := True;
       FChangeDetails := True;
    end else if Field.DisplayName = 'Amount' then begin
       FAmount  := Field.DisplayText;
       FChangeAmount  := True;
       FChangeDetails := False;
    end else if Field.DisplayName = 'Details' then begin
       FDetails := Field.DisplayText;
       FChangeAmount  := False;
       FChangeDetails := True;
    end;
    while POS('$',FAmount) > 0 do // remove the dollar
       delete(FAmount,POS('$',FAmount),1);
    SetFields;
end;
{ **************************************************************************** }
procedure TfrmTenders.SetFields;
var AmountExt : extended;
    ExRate : extended;
    f,ChangeRoundValue : real;
begin
     FInProcess := True;
     FTotalAmount := 0.00;
     FRoundValue  := 0.00;
     dm.ibdsTenders.DisableControls;
     with dm.ibdsTenders do begin
        first;
        while not EOF do begin
           if FieldByName('TENDER_TYPE').AsString = FTenderType then begin
              Edit; // only for the last change of amount or details
              if FChangeAmount then begin
                 if FAmount <> '' then begin
                    AmountExt := StrToFloat(FAmount);
                    f := Frac(AmountExt) * 100; // work out if not 0 or 5 cents
                    FRoundValue := RoundValue[(trunc(f) mod 10)];
                    if FCash and (FRoundValue <> 0.00 ) then
                       AmountExt := AmountExt + FRoundValue;
                    FieldByName('AMOUNT').AsString := FloattoStrF(AmountExt,ffCurrency,7,2);
                 end else begin
                    FieldByName('AMOUNT').Clear; // clear Fields that are not Tenders (15 max)
                 end;
              end;
              if FChangeDetails then
                 FieldByName('DETAILS').AsString := FDetails;
              Post;
           end;
           // Work out the Total - exchange rate taken into account
           // on the same record
           ExRate    := FieldByName('EX_RATE').AsFloat;
           AmountExt := Global.RemoveSignSpaceConvert(FieldByName('AMOUNT').AsString);
           if AmountExt = -9999999.00 then exit;
           if AmountExt <> 0.00 then begin
              if ExRate > 0.00 then    // convert if there is an exchange Rate to NZ$
                 AmountExt := Global.RoundCurrency(AmountExt / ExRate);
           end;
           FTotalAmount := FTotalAmount + AmountExt;
           next;
        end;
        // adjust TempPayment which is actually dbePayment - it adjusts according
        // to the Amount of the Tenders - CashAmount is the original Amount
        dbeChange.Hint  := '';
        dbePayment.Hint := '';

        if FTotalAmount = 0.00 then begin
           // put back up to cashamount
           dbeChange.Caption  := '$0.00';
           dbePayment.Caption  := FloatToStrf(CashAmount,ffCurrency,7,2);
        end else if FTotalAmount > CashAmount then begin
           // if last payment was cash (FRoundValue ) and the difference was +0.01 or +0.02
           // and FTotalAmount  then let the payment go thru
           if (FRoundValue = ( FTotalAmount - CashAmount)) and (FRoundValue > 0.00) then begin
              dbePayment.Hint := Format('Payment is rounded by %f',[FRoundValue]);
              dbePayment.Caption  := '$0.00';
              dbeChange.Caption   := '$0.00';
              RndValue := FRoundValue;
           end else begin
              f := Frac(FTotalAmount - CashAmount) * 100;
              ChangeRoundValue := RoundValue[(trunc(f) mod 10)];
              // round the change if needed
              dbeChange.Caption  := FloatToStrf((FTotalAmount + ChangeRoundValue)  - CashAmount,ffCurrency,7,2);
              dbePayment.Caption := '$0.00';
              dbeChange.Hint := Format('Change is rounded by %f',[ChangeRoundValue]);
           end;
        end else if (FTotalAmount < CashAmount) and (FTotalAmount <> 0.00) then begin
           // if last payment was cash (FRoundValue ) and the difference was -0.01 or -0.02
           // and FTotalAmount  then let the payment go thru
           if FRoundValue < 0.00 then begin
              dbePayment.Hint := Format('Payment is rounded by %f',[FRoundValue]);
              dbePayment.Caption  := '$0.00';
              dbeChange.Caption   := '$0.00';
              RndValue := FRoundValue;
           end else begin
              dbePayment.Caption := FloatToStrf(CashAmount - FTotalAmount,ffCurrency,7,2);
              dbeChange.Caption  := '$0.00';
           end;
        end else begin
           dbeChange.Caption  := '$0.00';
           dbePayment.Caption := '$0.00';
        end;
        // reposition the field in the grid
        first;
        while not EOF do begin
           if FieldByName('TENDER_TYPE').AsString = FKeepName then
              break;
           Next;
        end;
     end;
     dm.ibdsTenders.EnableControls;
     FInProcess := False;
end;
{ **************************************************************************** }
procedure TfrmTenders.dbgTendersRowChanged(Sender: TObject);
// keep the Record
begin
     if not active then exit;
     if FInProcess then exit;
     FTenderType := (Sender as TwwDBGrid).Fields[0].DisplayText;
     if (FTenderType = 'Cash') or (FTenderType = 'CASH') or (FTenderType = 'cash') then
        FCash := True
     else FCash := False;
end;
{ **************************************************************************** }
procedure TfrmTenders.dbgTendersDblClick(Sender: TObject);
var ExRate : extended;
    AmountExt : extended;
begin
    // non tender field
    if dm.ibdsTenders.FieldByName('TENDER_TYPE').AsString = '' then exit;
    // if the field is filled then it cannot be overwritten
    AmountExt  := Global.RemoveSignSpaceConvert(dbgTenders.GetActiveField.DisplayText);
    if (AmountExt > 0.00) or (AmountExt = -9999999.00) then exit;
    // if there is any Due
    AmountExt  := Global.RemoveSignSpaceConvert(dbePayment.Caption);
    if AmountExt = -9999999.00 then exit; // error on conversion
    if (dbgTenders.GetActiveField.DisplayName = 'Amount') and
       (AmountExt <> 0.00) then begin
       ExRate := dm.ibdsTenders.FieldByName('EX_RATE').AsFloat;
       if ExRate > 0.00 then  // convert if there is an exchange Rate to dbeDue
          AmountExt := RoundCurrency(AmountExt * ExRate);
       FAmount  := FloatToStr(AmountExt);
       FChangeAmount  := True;
       FChangeDetails := False;
       FKeepName := (Sender as TwwDBGrid).Fields[0].DisplayText;
       SetFields;
    end;
end;
{ **************************************************************************** }
procedure TfrmTenders.menuExitClick(Sender: TObject);
begin
     modalResult := mrcancel;
end;
{ **************************************************************************** }
procedure TfrmTenders.FormKeyUp(Sender: TObject; var Key: Word; Shift: TShiftState);
begin
     inherited;
     case key of   
         VK_ESCAPE : menuExitClick(Sender);
     end;    
     if key = VK_F2 then dbgTendersDblClick(dbgTenders);
     if ( ssAlt in shift ) then begin
        case key of 
          88,120  : menuExitClick(Sender);  // X  Close
          79,111  : btnOKClick(Sender);  // O
       end;
    end;
end;
{ **************************************************************************** }
procedure TfrmTenders.dbgTendersCalcCellColors(Sender: TObject;
  Field: TField; State: TGridDrawState; Highlight: Boolean; AFont: TFont; ABrush: TBrush);
begin
     if (Field.FieldName = 'DETAILS') or (Field.FieldName = 'AMOUNT') then 
        ABrush.Color := Global.fcGridEdit; 
end;
{ **************************************************************************** }
procedure TfrmTenders.FormResize(Sender: TObject);
begin
     ResizeToolBar( Self ,Self.WindowState,TwoButtonWidth,FpnlWidth,pnlSpace);
end;

end.
