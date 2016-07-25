unit dmMenuMate;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  IBDatabase, Db, IBCustomDataSet, IBTable, IBQuery, IBSQL;

type
  TdmMM = class(TDataModule)
    MMDatabase: TIBDatabase;
    MMTransaction: TIBTransaction;
    tblMMLocations: TIBTable;
    tblMMLocationsLOCATION_KEY: TIntegerField;
    tblMMLocationsNAME: TIBStringField;
    tblMMLocationsLOCATION_TYPE: TIntegerField;
    tblMMLocationsPROFILE_KEY: TIntegerField;
    qryMMReport: TIBQuery;
    ibdsUpdateMemberPoints: TIBDataSet;
    IntegerField7: TIntegerField;
    FloatField1: TFloatField;
    FloatField2: TFloatField;
    FloatField3: TFloatField;
    FloatField4: TFloatField;
    FloatField5: TFloatField;
    DateField7: TDateField;
    FloatField6: TFloatField;
    IBStringField34: TIBStringField;
    ibdsMembersPoints: TIBDataSet;
    ibdsMembersPointsADJUSTMENT_TYPE: TIntegerField;
    ibdsMembersPointsEARNED: TIBBCDField;
    ibdsMembersPointsMEMBER: TIntegerField;
    ibdsMembersPointsPOINTS_TYPE: TIntegerField;
    ibdsMembersPointsREDEEMED: TIBBCDField;
    IBMenuMatePoints: TIBTransaction;
    ibdsUpdateMemberPointsDONATION: TIBBCDField;
    ibdsUpdateMemberPointsDONATION_MEMBER: TIntegerField;
    ibdsUpdateDonationMember: TIBDataSet;
    IntegerField1: TIntegerField;
    FloatField7: TFloatField;
    FloatField8: TFloatField;
    FloatField9: TFloatField;
    FloatField10: TFloatField;
    FloatField11: TFloatField;
    DateField1: TDateField;
    FloatField12: TFloatField;
    IBStringField1: TIBStringField;
    IBBCDField1: TIBBCDField;
    IntegerField2: TIntegerField;
    sqlMemberPointsKey: TIBSQL;
    ibdsMembersPointsMEMBERS_POINTS_KEY: TIntegerField;
    dsrDiscounts: TDataSource;
    tblMMDiscounts: TIBTable;
    tblMMDiscountsDISCOUNT_KEY: TIntegerField;
    tblMMDiscountsNAME: TIBStringField;
    tblMMDiscountsDESCRIPTION: TIBStringField;
    tblMMDiscountsMENU: TIBStringField;
    tblMMDiscountsAMOUNT: TIBBCDField;
    tblDiscounts: TIBTable;
    tblDiscountsDISCOUNT_KEY: TIntegerField;
    tblDiscountsNAME: TIBStringField;
    tblDiscountsDESCRIPTION: TIBStringField;
    tblDiscountsMENU: TIBStringField;
    tblDiscountsAMOUNT: TIBBCDField;
    tblDiscountsPERCENTAGE: TIBBCDField;
    tblMMDiscountsPERCENTAGE: TIBBCDField;
    procedure DataModuleDestroy(Sender: TObject);
    function MMConnected : Boolean;
    procedure MMCommit;
    procedure DataModuleCreate(Sender: TObject);
  private                   
    { Private declarations }
  public
    { Public declarations }
  end;

var
  dmMM: TdmMM;

implementation

{$R *.DFM}

{ ****************************************************************************}
procedure TdmMM.DataModuleDestroy(Sender: TObject);
begin
     if MMTransaction.InTransaction then
        MMTransaction.Commit;
     if MMDatabase.Connected then   
        MMDatabase.Close;
end;
{ ****************************************************************************}
function TdmMM.MMConnected : Boolean;
begin
     result := false;
     if MMDatabase.Connected then begin
        result := true;
        if not MMTransAction.InTransAction then
           MMTransAction.StartTransAction;
     end;      
end;                 
{ ****************************************************************************}
procedure TdmMM.MMCommit;
begin
     if dmMM.MMTransAction.InTransAction then
        dmMM.MMTransAction.Commit;
end;
{ ****************************************************************************}
procedure TdmMM.DataModuleCreate(Sender: TObject);
begin
end;
{ ****************************************************************************}
end.
