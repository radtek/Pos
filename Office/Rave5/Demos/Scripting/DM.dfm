object DataModule1: TDataModule1
  OldCreateOrder = False
  Left = 200
  Top = 107
  Height = 262
  Width = 394
  object tablCustomer: TTable
    DatabaseName = 'DBDEMOS'
    TableName = 'customer.db'
    Left = 24
    Top = 16
  end
  object tablOrders: TTable
    DatabaseName = 'DBDEMOS'
    FilterOptions = [foNoPartialCompare]
    TableName = 'orders.db'
    Left = 104
    Top = 16
  end
  object cxnCustomer: TRvTableConnection
    RuntimeVisibility = rtDeveloper
    Table = tablCustomer
    Left = 24
    Top = 80
  end
  object cxnOrders: TRvTableConnection
    RuntimeVisibility = rtDeveloper
    Table = tablOrders
    Left = 104
    Top = 80
  end
end
