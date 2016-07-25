object dmStockReportData: TdmStockReportData
  OldCreateOrder = False
  Left = 65532
  Top = 65532
  Height = 657
  Width = 1292
  object StockTrans: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 24
    Top = 8
  end
  object qrStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Description,'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      '   StockLocation.On_Order,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level'
      'From'
      '   Stock Inner Join StockLocation on'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Inner Join StockGroup on'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Inner Join StockCategory on'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Order By'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Description,'
      '   StockLocation.Location')
    Left = 104
    Top = 56
  end
  object ravStock: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStock
    Left = 224
    Top = 56
  end
  object qrStockMovement: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select'
      #9'StockTrans.Location,'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group,'
      '                StockTrans.Description,'
      '                StockTrans.Note,'
      #9'Sum(StockTrans.Unit_Cost * StockTrans.Qty) Total_Cost '
      'From'
      #9'StockTrans'
      'Where'
      #9'Created >= '#39'10/1/2003'#39' and'
      #9'Created < '#39'11/1/2003'#39' and'
      #9'Transaction_Type = '#39'Receipt'#39
      'Group By'
      #9'StockTrans.Location,'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group,'
      '                StockTrans.Description,'
      '                StockTrans.Note'
      'Order By'
      #9'StockTrans.Location,'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group')
    Left = 104
    Top = 152
  end
  object ravStockMovement: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockMovement
    Left = 224
    Top = 152
  end
  object qrStockValue: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Description,'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      
        '   StockLocation.On_Hand * StockLocation.Average_Cost Average_Co' +
        'st,'
      
        '   StockLocation.On_Hand * StockLocation.Latest_Cost Latest_Cost' +
        ','
      '   StockLocation.Average_Cost Single_Average_Cost, '
      '   StockLocation.Latest_Cost   Single_Latest_Cost,'
      '   cast('#39#39' as varchar(25)) Name,'
      '   cast('#39#39' as varchar(25)) Committed_User_Name,'
      '   cast('#39#39' as varchar(25)) Initialised_User_Name,'
      '   cast(current_timestamp as Timestamp) Committed_Time,'
      '   cast(current_timestamp as Timestamp) Initialised_Time  '
      'From'
      '   Stock Inner Join StockLocation on'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Inner Join StockGroup on'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Left Join StockCategory on'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Order By'
      '   StockCategory.Sort_Order,'
      '   StockGroup.Sort_Order,'
      '   Stock.Description,'
      '   StockLocation.Location')
    Left = 104
    Top = 104
  end
  object ravStockValue: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockValue
    Left = 224
    Top = 104
  end
  object qrStocktake: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9'StockLocation.Location,'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'Stock.Code,'
      #9'Barcode.Barcode,'
      #9'Stock.Description,'
      #9'StockLocation.On_Hand,'
      #9'Stock.Stocktake_Unit,'
      #9'StockLocation.On_Hand,'
      
        #9'Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Un' +
        'it_Size) As Numeric(15,4)) On_Order'
      'From'
      #9'StockCategory Inner Join StockGroup On'
      
        #9#9'StockCategory.Stock_Category_Key = StockGroup.Stock_Category_K' +
        'ey'
      #9'Inner Join Stock On'
      #9#9'StockGroup.Stock_Group_Key = Stock.Stock_Group_Key'
      #9'Inner Join StockLocation On'
      #9#9'StockLocation.Stock_Key = Stock.Stock_Key'
      #9'Left Join PurchaseStock On'
      
        #9#9'(PurchaseStock.Code = Stock.Code And PurchaseStock.Location = ' +
        'StockLocation.Location)'
      #9'Left Join Barcode on'
      #9#9'Stock.Stock_Key = Barcode.Stock_Key'
      'Where'
      #9'Stock.Enabled = '#39'T'#39' And'
      #9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null)'
      'Group By'
      #9'StockLocation.Location,'
      #9'StockCategory.Sort_Order,'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'StockGroup.Sort_Order,'
      #9'Stock.Code,'
      #9'Barcode.Barcode,'
      #9'Stock.Description,'
      #9'Stock.Description,'
      #9'Stock.Stocktake_Unit,'
      #9'StockLocation.On_Hand'
      'Order By'
      #9'StockLocation.Location,'
      #9'StockCategory.Sort_Order,'
      #9'StockGroup.Sort_Order,'
      #9'Stock.Description')
    Left = 328
    Top = 80
  end
  object ravStocktake: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStocktake
    Left = 416
    Top = 80
  end
  object qrContactDetails: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      '   Contact.Company_Name,'
      '   Contact.Phone,'
      '   Contact.Fax,'
      '   Contact.Mobile,'
      '   Contact.Email,'
      '   Contact.Location_Address,'
      '   Contact.Contact_Name,'
      '   Contact.Contact_Phone,'
      '   Contact.Bill_Name,'
      '   Contact.Bill_Attn,'
      '   Contact.Bill_Phone,'
      '   Contact.Bill_Address'
      'From'
      '   Contact'
      'Order By'
      '   Contact.Company_Name')
    Left = 104
    Top = 8
  end
  object ravContactDetails: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrContactDetails
    Left = 224
    Top = 8
  end
  object qrStockVariance: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9#9#9'Stock.Code,'
      #9#9#9'StockCategory.Stock_Category,'
      #9#9#9'StockGroup.Stock_Group,'
      #9#9#9'Stock.Description,'
      #9#9#9'StockLocation.Location,'
      #9#9#9'StockLocation.Average_Cost,'
      #9#9#9'StockLocation.Latest_Cost,'
      #9#9#9'StockLocation.On_Hand,'
      #9#9#9'StockLocation.Stocktake,'
      #9#9#9'StockLocation.Variance ,'
      
        #9#9#9'Cast(-StockLocation.Variance * StockLocation.Average_Cost As ' +
        'Numeric(15, 4)) Cost,'
      #9#9#9'StockLocation.Opening,'
      #9#9#9'StockLocation.Sales,'
      #9#9#9'StockLocation.Writeoff,'
      #9#9#9'StockLocation.Inwards,'
      #9#9#9'StockLocation.Transfer'
      #9#9'From'
      #9#9#9'StockCategory Left Outer Join StockGroup On'
      
        #9#9#9#9'StockCategory.Stock_Category_Key = StockGroup.Stock_Category' +
        '_Key'
      #9#9#9'Left Outer Join Stock On'
      #9#9#9#9'StockGroup.Stock_Group_Key = Stock.Stock_Group_Key'
      #9#9#9'Left Join StockLocation On'
      #9#9#9#9'Stock.Stock_Key = StockLocation.Stock_Key'
      #9#9'Where'
      #9#9#9'Stock.Enabled = '#39'T'#39' And'
      #9#9#9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null)'
      #9#9'Order By '
      #9#9#9'StockLocation.Location, '
      #9#9#9'StockCategory.Sort_Order,'
      #9#9#9'StockGroup.Sort_Order, '
      #9#9#9'Stock.Description')
    Left = 104
    Top = 344
  end
  object ravStockVariance: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockVariance
    Left = 224
    Top = 344
  end
  object qrStockReorder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9'Contact.Company_Name,'
      #9'SupplierStock.Latest_Cost,'
      #9'SupplierStock.Supplier_Code,'
      #9'StockLocation.Location,'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'Stock.Code,'
      #9'Stock.Description,'
      #9'StockLocation.Max_Level,'
      #9'StockLocation.Min_Level,'
      #9'StockLocation.On_Hand,'
      
        #9'Cast(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Un' +
        'it_Size) As Numeric(15,4)) On_Order,'
      #9'Stock.Stocktake_Unit,'
      #9'SupplierStock.Supplier_Unit,'
      #9'SupplierStock.Qty,'
      #9'SupplierStock.Min_Order_Qty'
      'From'
      #9'SupplierStock Inner Join Stock On'
      #9#9'SupplierStock.Stock_Key = Stock.Stock_Key'
      #9'Inner Join StockGroup On'
      #9#9'Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      #9'Inner Join StockCategory On'
      
        #9#9'StockGroup.Stock_Category_Key = StockCategory.Stock_Category_K' +
        'ey'
      #9'Inner Join StockLocation On'
      #9#9'StockLocation.Stock_Key = Stock.Stock_Key'
      #9'Inner Join Contact On'
      #9#9'SupplierStock.Supplier_Key = Contact.Contact_LK'
      #9'Left Join PurchaseStock On'
      
        #9#9'(PurchaseStock.Code = Stock.Code And PurchaseStock.Location = ' +
        'StockLocation.Location)'
      'Where'
      
        #9'(StockLocation.Max_Level <> 0 or StockLocation.Min_Level <> 0) ' +
        'and'
      #9'SupplierStock.Default_Order_Unit = '#39'T'#39' and'
      #9'SupplierStock.Preferred_Supplier = '#39'T'#39
      'Group By'
      #9'Contact.Company_Name,'
      #9'SupplierStock.Latest_Cost,'
      #9'SupplierStock.Supplier_Code,'
      #9'StockLocation.Location,'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'Stock.Code,'
      #9'Stock.Description,'
      #9'StockLocation.Max_Level,'
      #9'StockLocation.Min_Level,'
      #9'StockLocation.On_Hand,'
      #9'Stock.Stocktake_Unit,'
      #9'SupplierStock.Supplier_Unit,'
      #9'SupplierStock.Qty,'
      #9'SupplierStock.Min_Order_Qty'
      'Having'
      
        #9'Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_Si' +
        'ze) + StockLocation.On_Hand <= StockLocation.Min_Level Or'
      
        #9'(Sum(PurchaseStock.Qty_On_Order * PurchaseStock.Supplier_Unit_S' +
        'ize) Is Null And StockLocation.On_Hand <= StockLocation.Min_Leve' +
        'l)'
      'Order By'
      #9'Contact.Company_Name,'
      #9'SupplierStock.Latest_Cost,'
      #9'SupplierStock.Supplier_Code,'
      #9'StockLocation.Location,'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'Stock.Code,'
      #9'Stock.Description,'
      #9'StockLocation.Max_Level,'
      #9'StockLocation.Min_Level,'
      #9'StockLocation.On_Hand,'
      #9'Stock.Stocktake_Unit,'
      #9'SupplierStock.Supplier_Unit,'
      #9'SupplierStock.Qty,'
      #9'SupplierStock.Min_Order_Qty')
    Left = 104
    Top = 440
  end
  object ravStockReorder: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockReorder
    Left = 224
    Top = 440
  end
  object qrPurchaseOrder: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9'PurchaseOrder.Order_Number,'
      #9'Contact.Company_Name Supplier_Name,'
      #9'PurchaseOrder.Supplier_Address,'
      #9'PurchaseOrder.Supplier_FAX,'
      #9'PurchaseOrder.Delivery_Address,'
      #9'PurchaseOrder.Instructions,'
      #9'PurchaseOrder.Required,'
      #9'PurchaseStock.Code,'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'Stock.Description,'
      #9'PurchaseStock.Supplier_Unit,'
      #9'PurchaseStock.Supplier_Code,'
      #9'Stock.GST_Percent,'
      
        #9'Cast(Sum(PurchaseStock.Supplier_Unit_Qty) as Numeric(15,4)) Ord' +
        'er_Qty,'
      
        #9'Cast(Sum(PurchaseStock.Supplier_Unit_Qty * PurchaseStock.Suppli' +
        'er_Unit_Cost) as Numeric(15,4)) Total_Cost,'
      
        #9'Cast(Sum((PurchaseStock.Supplier_Unit_Qty * PurchaseStock.Suppl' +
        'ier_Unit_Cost) * (Stock.GST_Percent + 100) / 100) as Numeric(15,' +
        ' 4) ) Total_Cost_Inc'
      'From'
      #9'PurchaseOrder Left Join PurchaseStock on'
      #9#9'PurchaseOrder.Order_LK = PurchaseStock.Order_FK'
      #9#9#9'Left Join Stock On'
      #9#9#9#9'PurchaseStock.Code = Stock.Code'
      #9#9#9'Left Join StockGroup On'
      #9#9#9#9'Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      #9#9#9'Left Join StockCategory On'
      
        #9#9#9#9'StockGroup.Stock_Category_Key = StockCategory.Stock_Category' +
        '_Key'
      #9'Left Join Contact On'
      #9#9'PurchaseOrder.Contact_FK = Contact.Contact_LK'
      'Where'
      #9'PurchaseOrder.Order_LK = 2'
      'Group By'
      #9'PurchaseOrder.Order_Number,'
      #9'Contact.Company_Name,'
      #9'PurchaseOrder.Supplier_Address,'
      #9'PurchaseOrder.Supplier_FAX,'
      #9'PurchaseOrder.Delivery_Address,'
      #9'PurchaseOrder.Instructions,'
      #9'PurchaseOrder.Required,'
      #9'PurchaseStock.Code,'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'Stock.Description,'
      #9'PurchaseStock.Supplier_Unit,'
      #9'PurchaseStock.Supplier_Code,'
      #9'Stock.GST_Percent'
      'Order By'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'Stock.Description,'
      #9'PurchaseStock.Supplier_Unit')
    Left = 328
    Top = 176
  end
  object ravPurchaseOrder: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrPurchaseOrder
    Left = 416
    Top = 176
  end
  object qrStockReceiptByCat: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Code,'
      '   Description,'
      '   Location,'
      '   Created,'
      '   Supplier_Name,'
      '   Reference,'
      '   Qty,'
      '   Unit,'
      '   StockTrans.Unit_Cost * StockTrans.Qty Total_Cost, '
      
        '   Cast((StockTrans.Unit_Cost * StockTrans.Qty) * (StockTrans.GS' +
        'T_Percent + 100) / 100 as Numeric(15, 4)) Total_Cost_Inc'
      'From'
      '   StockTrans Left Join Stock on'
      '      StockTrans.Code = Stock.Code'
      '   Left Join StockGroup on'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Left Join StockCategory on'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Where'
      '   Transaction_Type = '#39'Receipt'#39
      'Order By'
      '   StockTrans.Supplier_Name,'
      '   StockTrans.Created,'
      '   StockTrans.Reference,'
      '   StockCategory.Stock_Category,'
      '   StockTrans.Description')
    Left = 104
    Top = 200
  end
  object ravStockReceiptByCat: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockReceiptByCat
    Left = 224
    Top = 200
  end
  object qrStockTransferFrom: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9'StockTrans.Location From_Location,'
      #9'StockTrans.Purchaser_Name To_Location,'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group,'
      #9'StockTrans.Description,'
      #9'StockTrans.Unit,'
      #9'-Sum(StockTrans.Qty) Qty,'
      #9'-Sum(StockTrans.Unit_Cost * StockTrans.Qty) Total_Cost,'
      '                 StockTrans.TRANSFER_ID,'
      '                StockTrans.Created  '
      'From'
      
        #9'StockTrans Left Join TransactionBatch On StockTrans.Batch_Key =' +
        ' TransactionBatch.Batch_Key'
      'Where'
      #9'TransactionBatch.Created >= '#39'01/01/2004'#39' and'
      #9'TransactionBatch.Created < '#39'08/01/2015'#39' and'
      #9'TransactionBatch.Transaction_Type = '#39'Transfer'#39
      'Group By'
      #9'StockTrans.Location,'
      #9'StockTrans.Purchaser_Name,'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group,'
      #9'StockTrans.Description,'
      #9'StockTrans.Unit,'
      '                StockTrans.Created '
      'Order By'
      #9'StockTrans.Location,'
      #9'StockTrans.Purchaser_Name,'
      #9'StockTrans.Description')
    Left = 104
    Top = 248
  end
  object ravStockTransferFrom: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockTransferFrom
    Left = 224
    Top = 248
  end
  object qrStockGroupVariance: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9#9#9'StockLocation.Location,'
      #9#9#9'StockCategory.Stock_Category,'
      #9#9#9'StockGroup.Stock_Group,'
      
        #9#9#9'Sum(Cast(StockLocation.Average_Cost * StockLocation.Opening A' +
        's Numeric(15, 4))) Opening,'
      
        #9#9#9'Sum(Cast(StockLocation.Average_Cost * StockLocation.Inwards A' +
        's Numeric(15, 4))) Inwards,'
      
        #9#9#9'Sum(Cast(StockLocation.Average_Cost * StockLocation.Transfer ' +
        'As Numeric(15, 4))) Transfer,'
      
        #9#9#9'Sum(Cast(StockLocation.Average_Cost * StockLocation.Stocktake' +
        ' As Numeric(15, 4))) Closing,'
      
        #9#9#9'Sum(Cast((StockLocation.Opening + StockLocation.Inwards + Sto' +
        'ckLocation.Transfer - StockLocation.Stocktake)'
      #9#9#9#9' * StockLocation.Average_Cost As Numeric(15, 4))) Usage, '
      
        #9#9#9'Sum(Cast((StockLocation.Opening + StockLocation.Inwards + Sto' +
        'ckLocation.Transfer - StockLocation.Stocktake)'
      
        #9#9#9#9' * StockLocation.Assessed_Value As Numeric(15, 4))) Assessed' +
        '_Total '
      #9#9'From '
      #9#9#9'StockCategory Inner Join StockGroup On '
      
        #9#9#9#9'StockCategory.Stock_Category_Key = StockGroup.Stock_Category' +
        '_Key '
      #9#9#9'Inner Join Stock On '
      #9#9#9#9'StockGroup.Stock_Group_Key = Stock.Stock_Group_Key '
      #9#9#9'Inner Join StockLocation On '
      #9#9#9#9'Stock.Stock_Key = StockLocation.Stock_Key '
      #9#9'Where '
      #9#9#9'Stock.Enabled = '#39'T'#39' And '
      #9#9#9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null) And '
      #9#9#9'StockLocation.Stocktake Is Not Null'
      #9#9'Group By'
      #9'StockLocation.Location,'
      #9'StockCategory.Stock_Category,'
      #9'StockCategory.Sort_Order,'
      #9'StockGroup.Stock_Group,'
      #9'StockGroup.Sort_Order'
      #9#9'Order By '
      #9#9#9'StockLocation.Location,'
      #9#9#9'StockCategory.Sort_Order,'
      #9#9#9'StockGroup.Sort_Order')
    Left = 104
    Top = 392
  end
  object ravStockGroupVariance: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockGroupVariance
    Left = 224
    Top = 392
  end
  object qrStockInquiry: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9'StockTrans.Created,'
      #9'Extract (Year From StockTrans.Created) Trans_Year,'
      #9'Extract (Month From StockTrans.Created) Trans_Month,'
      #9'Extract (Day From StockTrans.Created) Trans_Day,'
      #9'StockTrans.Transaction_Type,'
      #9'StockTrans.Location,'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group,'
      #9'StockTrans.Description,'
      #9'StockTrans.Unit,'
      #9'StockTrans.Qty,'
      #9'StockTrans.Unit_Cost * StockTrans.Qty Total_Cost,'
      #9'StockTrans.User_Name,'
      #9'StockTrans.Supplier_Name,'
      #9'StockTrans.Reference,'
      #9'StockTrans.Note'
      'From'
      #9'StockTrans'
      'Where'
      #9'Created >= '#39'06/1/2004'#39' and'
      #9'Created < '#39'06/10/2004'#39
      'Order By'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group,'
      #9'StockTrans.Description,'
      #9'2, 3, 4,'
      #9'StockTrans.Transaction_Type')
    Left = 104
    Top = 296
  end
  object ravStockInquiry: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockInquiry
    Left = 224
    Top = 296
  end
  object qrSupplierBackOrders: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      '   Contact.Company_Name Supplier_Name,'
      '   PurchaseOrder.Order_Number,'
      '   PurchaseOrder.Created,'
      '   PurchaseStock.Location,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   PurchaseStock.Code,'
      '   Stock.Description,'
      '   PurchaseStock.Supplier_Unit,'
      '   PurchaseStock.Supplier_Unit_Qty,'
      
        '   PurchaseStock.Supplier_Unit_Qty - PurchaseStock.Qty_Received ' +
        '- PurchaseStock.Qty_Not_Received To_Receive'
      'From'
      '   PurchaseStock Left Join PurchaseOrder On'
      '      PurchaseStock.Order_FK = PurchaseOrder.Order_LK'
      '   Left Join Contact On'
      '      PurchaseOrder.Contact_FK = Contact.Contact_LK'
      '   Left Join Stock On'
      '      PurchaseStock.Code = Stock.Code'
      '   Left Join StockGroup On'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Left Join StockCategory On'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Where'
      '   Qty_Received + Qty_Not_Received < Supplier_Unit_Qty'
      'Order By'
      '   Contact.Company_Name,'
      '   PurchaseOrder.Order_Number,'
      '   PurchaseStock.Location,'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   PurchaseStock.Code,'
      '   Stock.Description')
    Left = 104
    Top = 488
  end
  object ravSupplierBackOrders: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSupplierBackOrders
    Left = 224
    Top = 488
  end
  object qrStockBarcodes: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9'StockCategory.Stock_Category,'
      #9'StockCategory.Sort_Order,'
      #9'StockGroup.Stock_Group,'
      #9'StockGroup.Sort_Order,'
      #9'Stock.Code,'
      #9'Stock.Stocktake_Unit,'
      #9'Stock.Description,'
      #9'Barcode.Barcode,'
      #9'Cast (Null as VarChar(25)) Supplier_Unit,'
      #9'Cast (Null as VarChar(50)) Company_Name'
      'From'
      #9'Stock Left Join StockGroup on'
      #9#9'Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      #9'Left Join StockCategory on'
      
        #9#9'StockGroup.Stock_Category_Key = StockCategory.Stock_Category_K' +
        'ey'
      #9'Left Join Barcode on'
      #9#9'Stock.Stock_Key = Barcode.Stock_Key'
      'Where'
      #9'Stock.Enabled = '#39'T'#39' And'
      #9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null)'
      ''
      'Union All'
      ''
      'Select'
      #9'StockCategory.Stock_Category,'
      #9'StockCategory.Sort_Order,'
      #9'StockGroup.Stock_Group,'
      #9'StockGroup.Sort_Order,'
      #9'Stock.Code,'
      #9'Stock.Stocktake_Unit,'
      #9'Stock.Description,'
      #9'Barcode.Barcode,'
      #9'SupplierStock.Supplier_Unit,'
      #9'Contact.Company_Name'
      'From'
      #9'Stock Left Join StockGroup on'
      #9#9'Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      #9'Left Join StockCategory on'
      
        #9#9'StockGroup.Stock_Category_Key = StockCategory.Stock_Category_K' +
        'ey'
      #9'Left Join SupplierStock on'
      #9#9'Stock.Stock_Key = SupplierStock.Stock_Key'
      #9'Left Join Barcode on'
      #9#9'Barcode.Supplier_Stock_Key = SupplierStock.Supplier_Stock_Key'
      #9'Left Join Contact on'
      #9#9'Contact.Contact_LK = SupplierStock.Supplier_Key'
      'Where'
      #9'Stock.Enabled = '#39'T'#39' And'
      #9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null) And'
      #9'Barcode.Barcode Is Not Null'
      'Order By'
      #9'2,'
      #9'4,'
      #9'7')
    Left = 464
    Top = 280
  end
  object ravStockBarcodes: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockBarcodes
    Left = 560
    Top = 280
  end
  object ravStockWriteOff: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockWriteOff
    Left = 552
    Top = 440
  end
  object qrStockWriteOff: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      'Select'
      #9'StockTrans.Created,'
      #9'Extract (Year From StockTrans.Created) Trans_Year,'
      #9'Extract (Month From StockTrans.Created) Trans_Month,'
      #9'Extract (Day From StockTrans.Created) Trans_Day,'
      '                StockTrans.Total_Cost,'
      #9'StockTrans.Transaction_Type,'
      #9'StockTrans.Location,'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group,'
      #9'StockTrans.Description,'
      #9'StockTrans.Unit,'
      #9'StockTrans.Qty,'
      #9'StockTrans.User_Name,'
      #9'StockTrans.Supplier_Name,'
      #9'StockTrans.Reference,'
      #9'StockTrans.Note'
      'From'
      #9'StockTrans'
      'Where'
      #9'Created >= '#39'06/1/2004'#39' and'
      #9'Created < '#39'06/10/2004'#39' and '
      '                Transaction_Type = '#39'Writeoff'#39
      ''
      'Order By'
      #9'StockTrans.Stock_Category,'
      #9'StockTrans.Stock_Group,'
      #9'StockTrans.Description,'
      #9'2, 3, 4,'
      #9'StockTrans.Transaction_Type')
    Left = 400
    Top = 432
  end
  object qrStockManufacture: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    SQL.Strings = (
      '        Select '
      #9#9#9'StockTrans.Description,'
      #9#9#9'StockTrans.Created,'
      #9#9#9'StockTrans.Location,'
      #9#9#9'StockTrans.Qty,'
      '            StockTrans.Total_Cost,'
      '            StockTrans.Unit_Cost,'
      '            StockTrans.Unit, '
      '            StockTrans.Note'
      #9#9'From '
      #9#9#9'StockTrans '
      ' '#9#9'Where '
      ' '#9#9#9'StockTrans.Created >= :StartTime and '
      ' '#9#9'                StockTrans.Created < :EndTime and '
      '            StockTrans.Transaction_Type = '#39'Manufacture'#39' '
      '        Order By '
      '            Note, '
      '            StockTrans.Created,'
      '            Qty DESC;')
    Left = 400
    Top = 231
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'StartTime'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'EndTime'
        ParamType = ptUnknown
      end>
  end
  object ravStockManufacture: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockManufacture
    Left = 520
    Top = 360
  end
  object qrExportedInvoices: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      '')
    Left = 336
    Top = 352
  end
  object ravExportedInvoices: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrExportedInvoices
    Left = 456
    Top = 351
  end
  object qrCommitedInvoices: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'SELECT '
      '    ST.TRANS_NUMBER,'
      '    ST.REFERENCE,'
      '    ST.BATCH_KEY,'
      '    ST.DESCRIPTION,'
      '    ST.CODE,'
      '    ST.STOCK_CATEGORY,'
      '    ST.ORDER_UNIT AS SUPPLIER_UNIT,'
      '    ST.STOCK_GROUP,'
      '    S.GST_PERCENT,'
      '    S.STOCK_KEY,'
      '    ST.GL_CODE,'
      '    ST.LOCATION,'
      '    ST.SUPPLIER_CODE,'
      '    ST.SUPPLIER_KEY,'
      '    ST.UNIT,'
      '    SS.LATEST_COST,'
      '    ST.ORDER_QTY,'
      '    SS.QTY AS SUPPLIER_UNIT_SIZE,'
      
        '    Cast((SS.LATEST_COST * ST.ORDER_QTY) as numeric(15, 4)) AS T' +
        'OTAL_COST'
      'FROM STOCKTRANS ST'
      'INNER JOIN STOCK S ON ST.CODE = S.CODE'
      
        'INNER JOIN SUPPLIERSTOCK SS ON S.STOCK_KEY = SS.STOCK_KEY AND ST' +
        '.SUPPLIER_KEY = SS.SUPPLIER_KEY'
      'WHERE ST.BATCH_KEY = :Batch_Key')
    Left = 528
    Top = 40
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Batch_Key'
        ParamType = ptUnknown
      end>
  end
  object ravCommitedInvoices: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrCommitedInvoices
    Left = 528
    Top = 156
  end
  object qrStockRequest: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      '   Contact.Company_Name,'
      '   Contact.Phone,'
      '   Contact.Fax,'
      '   Contact.Mobile,'
      '   Contact.Email,'
      '   Contact.Location_Address,'
      '   Contact.Contact_Name,'
      '   Contact.Contact_Phone,'
      '   Contact.Bill_Name,'
      '   Contact.Bill_Attn,'
      '   Contact.Bill_Phone,'
      '   Contact.Bill_Address'
      'From'
      '   Contact'
      'Order By'
      '   Contact.Company_Name')
    Left = 328
    Top = 8
  end
  object ravStockRequest: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrStockRequest
    Left = 408
    Top = 16
  end
  object qrSupplierInvoice: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Code,'
      '   Stock.Stocktake_Unit,'
      '   Stock.Description,'
      '   StockLocation.Location,'
      '   StockLocation.On_Hand,'
      '   StockLocation.On_Order,'
      '   StockLocation.Average_Cost,'
      '   StockLocation.Latest_Cost,'
      '   StockLocation.Min_Level,'
      '   StockLocation.Max_Level'
      'From'
      '   Stock Inner Join StockLocation on'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      '   Inner Join StockGroup on'
      '      Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      '   Inner Join StockCategory on'
      
        '      StockGroup.Stock_Category_Key = StockCategory.Stock_Catego' +
        'ry_Key'
      'Order By'
      '   StockCategory.Stock_Category,'
      '   StockGroup.Stock_Group,'
      '   Stock.Description,'
      '   StockLocation.Location'
      ''
      ''
      ' ')
    Left = 32
    Top = 64
  end
  object ravSupplierInvoice: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSupplierInvoice
    Left = 16
    Top = 128
  end
  object qrstockgroupnew: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      #9#9#9'StockLocation.Location,'
      #9#9#9'StockCategory.Stock_Category,'
      #9#9#9'StockGroup.Stock_Group,'
      
        #9#9#9'Sum(Cast(StockLocation.Average_Cost * StockLocation.Opening A' +
        's Numeric(15, 4))) Opening,'
      
        #9#9#9'Sum(Cast(StockLocation.Average_Cost * StockLocation.Inwards A' +
        's Numeric(15, 4))) Inwards,'
      
        #9#9#9'Sum(Cast(StockLocation.Average_Cost * StockLocation.Transfer ' +
        'As Numeric(15, 4))) Transfer,'
      
        #9#9#9'Sum(Cast(StockLocation.Average_Cost * StockLocation.Stocktake' +
        ' As Numeric(15, 4))) Closing,'
      
        #9#9#9'Sum(Cast((StockLocation.Opening + StockLocation.Inwards + Sto' +
        'ckLocation.Transfer - StockLocation.Stocktake)'
      #9#9#9#9' * StockLocation.Average_Cost As Numeric(15, 4))) Usage, '
      
        #9#9#9'Sum(Cast((StockLocation.Opening + StockLocation.Inwards + Sto' +
        'ckLocation.Transfer - StockLocation.Stocktake)'
      
        #9#9#9#9' * StockLocation.Assessed_Value As Numeric(15, 4))) Assessed' +
        '_Total '
      #9#9'From '
      #9#9#9'StockCategory Inner Join StockGroup On '
      
        #9#9#9#9'StockCategory.Stock_Category_Key = StockGroup.Stock_Category' +
        '_Key '
      #9#9#9'Inner Join Stock On '
      #9#9#9#9'StockGroup.Stock_Group_Key = Stock.Stock_Group_Key '
      #9#9#9'Inner Join StockLocation On '
      #9#9#9#9'Stock.Stock_Key = StockLocation.Stock_Key '
      #9#9'Where '
      #9#9#9'Stock.Enabled = '#39'T'#39' And '
      #9#9#9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null) And '
      #9#9#9'StockLocation.Stocktake Is Not Null'
      #9#9'Group By'
      #9'StockLocation.Location,'
      #9'StockCategory.Stock_Category,'
      #9'StockCategory.Sort_Order,'
      #9'StockGroup.Stock_Group,'
      #9'StockGroup.Sort_Order'
      #9#9'Order By '
      #9#9#9'StockLocation.Location,'
      #9#9#9'StockCategory.Sort_Order,'
      #9#9#9'StockGroup.Sort_Order')
    Left = 96
    Top = 536
  end
  object ravstockgroupnew: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrstockgroupnew
    Left = 216
    Top = 544
  end
  object qrTransferAudit: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trTime
    SQL.Strings = (
      'Select '
      '            STOCKTRANS.TRANSFER_ID, '
      '           STOCKTRANS.Location   From_Location,'
      '          StockTrans.Purchaser_Name To_Location,'
      '           STOCKTRANS.Created,'
      '           STOCKTRANS.REFERENCE,'
      '           STOCKTRANS.USER_ID,'
      '             STOCKTRANS.Description,'
      '            STOCKTRANS.UNIT,'
      '           STOCKTRANS.UNIT_COST,'
      '            STOCKTRANS.Stock_Category,'
      '           STOCKTRANS.Stock_Group,'
      '           STOCKTRANS.STOCK_REQUEST_NO,'
      '            STOCKTRANS.Qty ,'
      #9' Sum(StockTrans.Unit_Cost * StockTrans.Qty) Total_Cost,'
      #9'      STOCKREQUEST_STOCK.QTY_REQUIRED'
      ''
      ' From '
      
        '       STOCKTRANS inner join     STOCKREQUEST_STOCK on STOCKTRAN' +
        'S.STOCK_REQUEST_STOCK_KEY_VAL = STOCKREQUEST_STOCK.STOCK_REQUEST' +
        '_STOCKKEY'
      '  Where '
      '      STOCKTRANS.TRANSFER_ID = 14 and STOCKTRANS.Qty>0 '
      ''
      'Group By '
      '        STOCKTRANS.Description,'
      '        STOCKTRANS.TRANSFER_ID, '
      '        StockTrans.Purchaser_Name,'
      '        STOCKTRANS.Location,'
      '        STOCKTRANS.Created,'
      '        STOCKTRANS.REFERENCE,'
      '        STOCKTRANS.USER_ID,'
      '        STOCKTRANS.UNIT,'
      '        STOCKTRANS.UNIT_COST,'
      '        STOCKTRANS.Stock_Category,'
      '        STOCKTRANS.Stock_Group,'
      '        STOCKTRANS.STOCK_REQUEST_NO,'
      '        STOCKREQUEST_STOCK.QTY_REQUIRED,'
      '        STOCKTRANS.Qty,'
      '        STOCKTRANS.Total_Cost'
      ''
      'Order By '
      
        '              STOCKTRANS.STOCK_REQUEST_NO, StockTrans.Descriptio' +
        'n  ')
    Left = 672
    Top = 447
  end
  object ravTransferAudit: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrTransferAudit
    Left = 768
    Top = 448
  end
  object qrStockReconcialation: TIBQuery
    Database = dmStockData.dbStock
    Transaction = dmStockData.trTime
    Left = 824
    Top = 319
  end
  object ravStockReconcialation: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrStockReconcialation
    Left = 928
    Top = 432
  end
end
