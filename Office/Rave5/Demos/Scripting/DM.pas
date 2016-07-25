unit DM;

interface

uses
  SysUtils, Classes, RpDefine, RpCon, RpConDS, RpConBDE, DB, DBTables;

type
  TDataModule1 = class(TDataModule)
    tablCustomer: TTable;
    tablOrders: TTable;
    cxnCustomer: TRvTableConnection;
    cxnOrders: TRvTableConnection;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  DataModule1: TDataModule1;

implementation

{$R *.dfm}

end.