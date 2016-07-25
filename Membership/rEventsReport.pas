unit rEventsReport;
{SELECT M.MEMBER,M.FULL_NAME,M.ADDRESS,M.DISTRICT,M.CITY,M.COUNTRY,M.ADDRESS1,ME.DATES,ME.EVENT,E.DESCRIPTION from MEMBERS M,M_EVENTS ME,EVENTS E,M_GROUPS mg where M.MEMBER = Mg.MEMBER and M.MEMBER = ME.MEMBER AND ME.EVENT = E.EVENT and extract( month from ME.DATES) = 1 and ME.DATES <> '12/30/1899'
  and SUB_PAID > 0
 ORDER by ME.EVENT,M.MEMBER 
}
interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Qrctrls, QuickRpt, ExtCtrls;

type
  TfrmEventsReport = class(TForm)
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
    QRDBText9: TQRDBText;
    QRDBText4: TQRDBText;
    QRBand2: TQRBand;
    lblLogo: TQRLabel;
    QRGroup1: TQRGroup;
    QRDBText5: TQRDBText;
    QRLabel2: TQRLabel;
    lblAddress: TQRLabel;
    QRLabel1: TQRLabel;
    lblPaid: TQRLabel;
    procedure QuickRepBeforePrint(Sender: TCustomQuickRep;
           var PrintReport: Boolean);
   function Extract(  qrprint : Boolean ;PrtIndex,mMonths : smallInt ; eEvent,mGroups  : String; Members : shortstring; SortBy : Integer ) : Boolean;
    procedure DetailBandBeforePrint(Sender: TQRCustomBand;
      var PrintBand: Boolean);
  private
    { Private declarations }
     FMonths : smallInt;
  public
    { Public declarations }
  end;

var
  frmEventsReport: TfrmEventsReport;

implementation

{$R *.DFM}
Uses
    dmMembers, //StockMaster
    Global; //StockMaster
var
    Printing : Boolean;

{ **************************************************************************** }
procedure TfrmEventsReport.QuickRepBeforePrint(Sender: TCustomQuickRep;  var PrintReport: Boolean);
begin
    lblHeading.Caption := Global.PrintLogo;
end;
{ **************************************************************************** }
function TfrmEventsReport.Extract( qrprint : Boolean ;PrtIndex,mMonths : smallInt ; eEvent,mGroups : String; Members : shortstring ; SortBy : Integer) : Boolean;
var S : String;
begin
     // NB tried to use Groups and Events - cant - uses the same list
     // only picks up Members ho have a group - if all groups are checked

     result := False;
     FMonths := mMonths + 1; // 0 based array
     with dm.qryReport do begin
         close;
         SQL.Text :=  format('SELECT M.MEMBER,M.FULL_NAME,FIRST_NAME,M.ADDRESS,M.DISTRICT,M.CITY,M.COUNTRY,M.ADDRESS1,ME.DATES,' +
                      'ME.EVENT,E.DESCRIPTION,M.KNOWN_AS,m.SURNAME,m.SUB_PAID from MEMBERS M,M_EVENTS ME,EVENTS E,M_GROUPS mg ' +
                      'where M.MEMBER = Mg.MEMBER and M.MEMBER = ME.MEMBER AND ME.EVENT = E.EVENT ' +
                      'and extract( month from ME.DATES) = %d and ME.DATES <> ''12/30/1899''',[FMonths]);
         if mGroups <> ''  then SQL.Add( Format( ' AND mg.GROUPS in ( %s )',[mGroups]));
         //if eEvent <> ''  then SQL.Add( Format( ' AND ME.EVENT in ( %s )',[eEvent]));
         if Members <> '' then SQL.Add( Format( ' %s',[Members]));
         case SortBy of
            0 : s := 'M.MEMBER';
            1 : s := 'M.FULL_NAME';
            2 : s := 'M.SURNAME';
         end;
         SQL.Add( format(' ORDER by ME.EVENT,%S',[S]));
         //AddToClipBoard(SQL.Text);
         open;
     end;
     
     qr.PrinterSettings.PrinterIndex := PrtIndex;
     Printing := qrPrint;
     result := True;
end;
{ **************************************************************************** }
procedure TfrmEventsReport.DetailBandBeforePrint(Sender: TQRCustomBand;   var PrintBand: Boolean);
begin
     with dm.qryReport do begin
        lblPaid.Caption := YesNoTypesArray[FieldByName('SUB_PAID').AsInteger];
        lblAddress.Caption := GetAddressAsString(FieldByName('ADDRESS').AsString,
                                                 FieldByName('DISTRICT').AsString,
                                                 FieldByName('ADDRESS1').AsString,
                                                 FieldByName('CITY').AsString,
                                                 FieldByName('COUNTRY').AsString);
                                                 
     end;                         
end;
{ **************************************************************************** }
end.
