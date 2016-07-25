unit rIndividualMember;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  QuickRpt, Qrctrls, ExtCtrls;

type
  TfrmIndividualMember = class(TForm)
    qr: TQuickRep;
    QRBand1: TQRBand;
    QRSysData1: TQRSysData;
    QRSysData2: TQRSysData;
    lblHeading: TQRLabel;
    lblReportTitle: TQRLabel;
    QRLabel10: TQRLabel;
    QRBand3: TQRBand;
    dbtMember: TQRDBText;
    dbtFullName: TQRDBText;
    dbtAddress: TQRDBText;
    QRLabel1: TQRLabel;
    QRDBText2: TQRDBText;
    QRLabel2: TQRLabel;
    QRLabel3: TQRLabel;
    QRDBText3: TQRDBText;
    QRDBText4: TQRDBText;
    QRDBText5: TQRDBText;
    QRDBText6: TQRDBText;
    QRLabel4: TQRLabel;
    QRLabel5: TQRLabel;
    QRLabel6: TQRLabel;
    QRLabel7: TQRLabel;
    QRLabel8: TQRLabel;
    QRLabel9: TQRLabel;
    QRDBText7: TQRDBText;
    QRDBText1: TQRDBText;
    QRDBText8: TQRDBText;
    QRDBText9: TQRDBText;
    QRLabel11: TQRLabel;
    QRLabel12: TQRLabel;
    QRLabel13: TQRLabel;
    QRDBText11: TQRDBText;
    QRLabel15: TQRLabel;
    QRDBText12: TQRDBText;
    QRLabel16: TQRLabel;
    QRDBText13: TQRDBText;
    QRLabel18: TQRLabel;
    QRLabel19: TQRLabel;
    QRLabel20: TQRLabel;
    QRLabel21: TQRLabel;
    QRLabel22: TQRLabel;
    QRLabel23: TQRLabel;
    QRLabel24: TQRLabel;
    QRLabel25: TQRLabel;
    QRDBText15: TQRDBText;
    QRDBText16: TQRDBText;
    QRDBText17: TQRDBText;
    QRDBText18: TQRDBText;
    QRDBText19: TQRDBText;
    QRDBText20: TQRDBText;
    QRLabel17: TQRLabel;
    QRLabel26: TQRLabel;
    QRLabel27: TQRLabel;
    QRLabel28: TQRLabel;
    QRLabel29: TQRLabel;
    QRLabel30: TQRLabel;
    QRLabel31: TQRLabel;
    QRLabel32: TQRLabel;
    QRLabel33: TQRLabel;
    QRLabel34: TQRLabel;
    QRLabel36: TQRLabel;
    QRLabel37: TQRLabel;
    QRDBText29: TQRDBText;
    QRDBText34: TQRDBText;
    QRLabel38: TQRLabel;
    QRLabel39: TQRLabel;
    QRDBText35: TQRDBText;
    QRDBText21: TQRDBText;
    QRDBText38: TQRDBText;
    QRDBText39: TQRDBText;
    QRDBText40: TQRDBText;
    QRDBText41: TQRDBText;
    QRDBText42: TQRDBText;
    QRDBText43: TQRDBText;
    QRDBText44: TQRDBText;
    QRDBText37: TQRDBText;
    QRDBText36: TQRDBText;
    QRDBText28: TQRDBText;
    QRDBText27: TQRDBText;
    QRDBText26: TQRDBText;
    QRDBText25: TQRDBText;
    QRDBText24: TQRDBText;
    QRDBText23: TQRDBText;
    QRDBText22: TQRDBText;
    QRLabel50: TQRLabel;
    QRLabel51: TQRLabel;
    QRLabel52: TQRLabel;
    QRLabel53: TQRLabel;
    QRLabel54: TQRLabel;
    QRLabel55: TQRLabel;
    QRLabel56: TQRLabel;
    QRLabel57: TQRLabel;
    QRDBText52: TQRDBText;
    QRDBText50: TQRDBText;
    QRDBText49: TQRDBText;
    QRDBText48: TQRDBText;
    QRDBText30: TQRDBText;
    QRDBText31: TQRDBText;
    QRDBText32: TQRDBText;
    QRDBText33: TQRDBText;
    QRLabel43: TQRLabel;
    QRLabel42: TQRLabel;
    QRLabel48: TQRLabel;
    QRLabel45: TQRLabel;
    QRLabel44: TQRLabel;
    QRLabel41: TQRLabel;
    QRLabel40: TQRLabel;
    QRLabel49: TQRLabel;
    QRDBText55: TQRDBText;
    QRDBText51: TQRDBText;
    QRLabel58: TQRLabel;
    QRDBText45: TQRDBText;
    QRLabel59: TQRLabel;
    QRLabel60: TQRLabel;
    QRLabel61: TQRLabel;
    QRDBText53: TQRDBText;
    QRDBText56: TQRDBText;
    QRDBText57: TQRDBText;
    QRLabel62: TQRLabel;
    QRLabel63: TQRLabel;
    QRLabel64: TQRLabel;
    QRLabel65: TQRLabel;
    QRLabel66: TQRLabel;
    QRLabel67: TQRLabel;
    QRLabel68: TQRLabel;
    QRLabel69: TQRLabel;
    QRDBText59: TQRDBText;
    QRDBText60: TQRDBText;
    QRDBText61: TQRDBText;
    QRDBText62: TQRDBText;
    QRDBText63: TQRDBText;
    QRDBText64: TQRDBText;
    QRShape1: TQRShape;
    QRShape2: TQRShape;
    QRShape3: TQRShape;
    QRLabel14: TQRLabel;
    QRDBText10: TQRDBText;
    QRShape4: TQRShape;
    QRShape5: TQRShape;
    RichText: TQRRichText;
    QRShape6: TQRShape;
    lblValueofCredit: TQRLabel;
    QRLabel35: TQRLabel;
    lblFinancial: TQRLabel;
    QRDBText14: TQRDBText;
    QRDBRichText1: TQRDBRichText;
    procedure qrBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
    procedure Setup( Member : LongInt );
    procedure QRBand3BeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private                                           
    { Private declarations }
    fValueOfCredit : String[12];
  public
    { Public declarations }
  end;

var
  frmIndividualMember: TfrmIndividualMember;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster


procedure TfrmIndividualMember.qrBeforePrint(Sender: TCustomQuickRep; var PrintReport: Boolean);
begin                                     
     lblHeading.Caption := Global.PrintLogo;
end;
{ **************************************************************************** }
procedure TfrmIndividualMember.Setup( Member : LongInt );
const SQL1 = 'Select * from MEMBERS where MEMBER = %d';
      SQL2 = 'Select mg.GROUPS,g.DESCRIPTION from M_GROUPS mg,GROUPS g where mg.MEMBER = %d ' +
             'and mg.GROUPS = g.GROUPS';
      SQL3 = 'Select ms.SUBSCRIPTION,s.DESCRIPTION,ms.RECEIPT,ms.AMOUNT,ms.AMOUNT_PAID ' +
             'from M_SUBS ms,SUBSCRIPTIONS s where ms.MEMBER = %d ' +
             'and ms.SUBSCRIPTION = s.SUBSCRIPTION';
      SQL4 = 'Select BONUS_CREDIT + EARNT_CREDIT + ' +
             'PRE_PAID_CREDIT as VALUE_OF_CREDIT from MEMBERS where MEMBER = %d ';
begin
      with dm.qryGeneral do begin
         dm.sqlExecquery(dm.qryGeneral,format(SQL4,[Member]));
         fValueOfCredit   := FormatFloat('$####0.00',FieldByName('VALUE_OF_CREDIT').AsFloat);
         //fPointsAvailable := FormatFloat('$####0.00',FieldByName('POINTS_AVAILABLE').AsFloat);
         RichText.Lines.Clear;
         dm.sqlExecquery(dm.qryGeneral,format(SQL2,[Member]));
         while not EOF do begin
            RichText.Lines.Add('Group  ' + FieldByName('GROUPS').AsString + '   ' +
                               FieldByName('DESCRIPTION').AsString);
            next;                   
         end;
         RichText.Lines.Add('');
         dm.sqlExecquery(dm.qryGeneral,format(SQL3,[Member]));
         while not dm.qryGeneral.EOF do begin
            RichText.Lines.Add('Subscription   ' + FieldByName('SUBSCRIPTION').AsString + '     ' +
                               FieldByName('DESCRIPTION').AsString + '      Receipt  ' +
                               FieldByName('RECEIPT').AsString     + '      Amt Paid  ' +
                               FormatFloat('$###0.00',FieldByName('AMOUNT_PAID').AsFloat) + '     Fee  ' +
                               FormatFloat('$###0.00',FieldByName('AMOUNT').AsFloat ));
            next;                                                                      
         end;    
      end;
      with dm.qryReport do begin
         Close;
         SQL.Text := format (SQL1,[Member]);
         open;
      end;
end;
{ **************************************************************************** }
procedure TfrmIndividualMember.QRBand3BeforePrint(Sender: TQRCustomBand;  var PrintBand: Boolean);
begin
     lblValueOfCredit.Caption   := fValueOfCredit;
     lblFinancial.Caption       := YesNoTypesArray[dm.qryReport.FieldByName('SUB_PAID').AsInteger];
end;

end.
