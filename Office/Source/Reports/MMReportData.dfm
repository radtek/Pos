object dmMMReportData: TdmMMReportData
  OldCreateOrder = False
  OnDestroy = DataModuleDestroy
  Left = 65532
  Top = 65532
  Height = 715
  Width = 1378
  object qrMenu: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '   Menu.Menu_Name,'
      '   Course.Course_Name,'
      '   Item.Item_Name,'
      '   ItemSize.Size_Name,'
      '   ItemSize.Price,'
      '   ItemSize.Cost Menu_Cost,                                   '
      '   ItemSize.Special_Price,'
      '   ItemSize.Price - ItemSize.Special_Price Difference,'
      '   ItemSize.Points_Percent,'
      '   ItemSize.GST_Percent,'
      '   Recipe.Stock_Code Code,'
      '   Recipe.Qty,'
      '   Recipe.Stock_Location Location'
      'From'
      '   Menu Left Join Course On'
      '      Menu.Menu_Key = Course.Menu_Key'
      '   Left Join Item On'
      '      Course.Course_Key = Item.Course_Key'
      '   Left Join ItemSize On'
      '      Item.Item_Key = ItemSize.Item_Key'
      '   Left Join Recipe On'
      '      ItemSize.ItemSize_Key = Recipe.ItemSize_Key'
      'Order By'
      '   Menu.Menu_Name,'
      '   Course.Course_Name,'
      '   Item.Item_Name,'
      '   ItemSize.Size_Name')
    Left = 120
    Top = 8
  end
  object qrMenuStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    SQL.Strings = (
      'Select'
      '   Stock.Code,'
      '   Stock.Description,'
      '   Stock.Stocktake_Unit,'
      '   StockLocation.Average_Cost Cost,'
      '   StockLocation.Assessed_Value'
      'From'
      '   Stock Left Join StockLocation On'
      '      Stock.Stock_Key = StockLocation.Stock_Key'
      'Where'
      '   Stock.Code = '#39'1129'#39' And'
      '   StockLocation.Location = '#39'RESTAURANT'#39)
    Left = 128
    Top = 56
  end
  object ravMenu: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = cdsMenu
    Left = 232
    Top = 8
  end
  object ravMenuStock: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrMenuStock
    Left = 232
    Top = 56
  end
  object MMTrans: TIBTransaction
    DefaultDatabase = dmMMData.dbMenuMate
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 32
    Top = 8
  end
  object StockTrans: TIBTransaction
    DefaultDatabase = dmStockData.dbStock
    Params.Strings = (
      'read_committed'
      'rec_version'
      'nowait')
    Left = 32
    Top = 56
  end
  object qrCashupTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '    ArcBillPay.Pay_Type,'
      '     ArcBillPay.Group_number,'
      '    ArcBillPay.Properties,'
      '   Sum (ArcBillPay.SubTotal) SubTotal,'
      '    Count (ArcBillPay.ArcBillPay_Key) Trans_Count'
      'From'
      '    (Security Left Outer Join ArcBill On'
      '        Security.Security_Ref = ArcBill.Security_Ref)'
      '     Left Outer Join ArcBillPay On'
      '        ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key'
      'Where'
      '    Security.Time_Stamp >= '#39'7/28/2003'#39' And'
      '    Security.Time_Stamp < '#39'7/29/2003'#39' And'
      '    Security.Security_Event = '#39'Billed By'#39
      'Group By'
      '     ArcBillPay.Group_number,'
      '    ArcBillPay.Properties, '
      '   ArcBillPay.Pay_Type'
      'Having'
      '    Count (ArcBillPay.ArcBillPay_Key) > 0'
      'Order By'
      '    ArcBillPay.Pay_Type Asc')
    Left = 120
    Top = 312
  end
  object ravCashupTotal: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCashupTotal
    Left = 232
    Top = 312
  end
  object qrCashupFloat: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '   Terminal_Name,'
      '   Sum(Initial_Float) Initial_Float,'
      '   Sum(Skims_Total) Skims_Total'
      'From'
      '   Zeds'
      'Where'
      '   Time_Stamp >= '#39'7/28/2003'#39' And'
      '   Time_Stamp < '#39'9/28/2003'#39)
    Left = 120
    Top = 264
  end
  object ravCashupFloat: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCashupFloat
    Left = 232
    Top = 264
  end
  object qrCashup: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '    Security.Terminal_Name,'
      '    ArcBillPay.Pay_Type,'
      '    ArcBillPay.Group_number,'
      ''
      '    ArcBillPay.Properties,'
      '    Sum (ArcBillPay.SubTotal) SubTotal,'
      '    Count (ArcBillPay.ArcBillPay_Key) Trans_Count'
      'From'
      '    (Security Left Outer Join ArcBill On'
      '        Security.Security_Ref = ArcBill.Security_Ref)'
      '     Left Outer Join ArcBillPay On'
      '        ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key'
      'Where'
      '    Security.Time_Stamp >= '#39'7/28/2003'#39' And'
      '    Security.Time_Stamp < '#39'7/29/2003'#39' And'
      '    Security.Security_Event = '#39'Billed By'#39
      'Group By'
      '    Security.Terminal_Name,'
      '    ArcBillPay.Group_number,'
      ''
      '    ArcBillPay.Pay_Type,'
      '    ArcBillPay.Properties'
      'Having'
      '    Count (ArcBillPay.ArcBillPay_Key) > 0'
      'Order By'
      '     Security.Terminal_Name Asc,'
      '     ArcBillPay.Group_number,'
      ''
      '     ArcBillPay.Pay_Type Asc')
    Left = 120
    Top = 216
  end
  object ravCashup: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCashup
    Left = 232
    Top = 216
  end
  object qrSalesSummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'ArcBill.Billed_Location Location,'
      #9'CategoryGroups.Name Category_Group,'
      #9'Sum(Archive.Qty) Item_Count,'
      
        #9'Sum((Archive.Price) * 100 / (Archive.GST_Percent + 100)) PriceE' +
        'xc,'
      #9'Sum(Archive.Price) PriceInc,'
      #9'Sum(Archive.Cost) Cost'
      'From'
      #9'Security Left Join ArcBill on'
      #9#9'Security.Security_Ref = ArcBill.Security_Ref'
      #9'Left Join Archive on'
      #9#9'ArcBill.ArcBill_Key = Archive.ArcBill_Key'
      #9'Left Join ArcCategories on '
      #9#9'Archive.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where '
      #9'(Archive.Order_Type = 3 or'
      #9'Archive.Order_Type = 0) and '
      #9'Security.Time_Stamp >= '#39'1/1/2004'#39' and'
      #9'Security.Time_Stamp < '#39'1/2/2004'#39' and'
      #9'Security.Security_Event = '#39'Billed By'#39
      'Group By'
      #9'ArcBill.Billed_Location,'
      #9'CategoryGroups.Name'
      'Having'
      #9'Count(Archive.Archive_Key) > 0'
      'Order By'
      #9'1,2'
      '')
    Left = 120
    Top = 360
  end
  object ravSalesSummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSalesSummary
    Left = 232
    Top = 360
  end
  object qrTipsVouchers: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '   Security.Time_Stamp,'
      '   Contacts.Name,'
      '   ArcSurcharge.Pay_Type,'
      '   ArcSurcharge.Pay_Type_Details,'
      '   ArcSurcharge.SubTotal,'
      '   ArcBill.Invoice_Number'
      'From'
      '   ArcSurcharge Left Join ArcBill On'
      '      ArcSurcharge.Arcbill_Key = ArcBill.Arcbill_Key'
      '   Left Join Security On'
      '      ArcBill.Security_Ref = Security.Security_Ref'
      '   Left Join Contacts On'
      '      Security.User_Key = Contacts.Contacts_Key'
      'Where'
      '   Security.Time_Stamp >= '#39'11/7/2003'#39' And'
      '   Security.Time_Stamp < '#39'11/8/2003'#39' And'
      '   Security.Security_Event = '#39'Billed By'#39
      'Order By'
      '   ArcSurcharge.Pay_Type,'
      '   Security.Time_Stamp')
    Left = 120
    Top = 456
  end
  object ravTipsVouchers: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrTipsVouchers
    Left = 232
    Top = 456
  end
  object qrHalfHoulrySummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'ArcBill.ArcBill_Key,'
      #9'ArcBill.Billed_Location,'
      #9'Security.Time_Stamp,'
      #9'Extract (Minute From Security.Time_Stamp) / 30 Bill_Half_Hour,'
      #9'Extract (Minute From Security.Time_Stamp) Bill_Minute,'
      #9'Extract (Hour From Security.Time_Stamp) Bill_Hour,'
      #9'Extract (Day From Security.Time_Stamp) Bill_Day,'
      #9'Extract (Month From Security.Time_Stamp) Bill_Month,'
      #9'Extract (Year From Security.Time_Stamp) Bill_Year,'
      ''
      #9'CAST('#39'12/30/1899'#39' AS TIMESTAMP) +'
      
        #9'Cast(((Extract (Minute From Security.Time_Stamp) / 30) * 30 * 6' +
        '0) +'
      
        #9'(Extract (Hour From Security.Time_Stamp) * 60 * 60) As Double P' +
        'recision) / 86400 Start_Time,'
      ' '
      #9'CAST('#39'12/30/1899'#39' AS TIMESTAMP) +'
      
        #9'Cast(((Extract (Minute From Security.Time_Stamp) / 30) * 30 * 6' +
        '0) +'
      
        #9'(Extract (Hour From Security.Time_Stamp) * 60 * 60) + 1800 As D' +
        'ouble Precision) / 86400 End_Time,'
      ''
      #9'ArcBill.Total - ArcBill.Discount Bill_Total,'
      #9'ArcBill.Patron_Count'
      'From'
      #9'Security Left Join ArcBill on'
      #9#9'Security.Security_Ref = ArcBill.Security_Ref'
      'Where'
      #9'Security.Time_Stamp >= '#39'5/5/2003'#39' and'
      #9'Security.Time_Stamp < '#39'5/6/2003'#39' and'
      #9'Security.Security_Event = '#39'Billed By'#39
      'Order By'
      #9'ArcBill.Billed_Location,'
      #9'Time_Stamp')
    Left = 88
    Top = 400
  end
  object ravHalfHoulrySummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = cdsHalfHourlySummary
    Left = 232
    Top = 408
  end
  object qrWages: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      '        select '
      '           Contact_Type, '
      '           Name, '
      '           Payroll_ID, '
      '           Login_DateTime, '
      '           Logout_DateTime, '
      '           Breaks,'
      '           (tt - bd) * 24 * 60 * 60 Seconds_Worked, '
      '           (tt - bd) * 24 * 60 Minutes_Worked, '
      '           (tt - bd) * 24 Hours_Worked, '
      '           (tt - bd) Days_Worked, '
      '           Department, '
      '           Zone, '
      '           Modified '
      
        '           from (select (ct.logout_datetime - ct.login_datetime)' +
        ' tt, '
      '                        (cast(ct.breaks as timestamp) '
      
        '                         - cast((current_date || '#39', 00:00:00.000' +
        #39') as timestamp)) bd, '
      '                        C.Contact_Type, '
      '                        C.Name, '
      '                        C.Payroll_ID, '
      '                        ct.Login_DateTime, '
      '                        ct.Logout_DateTime, '
      '                        ct.Breaks,'
      '                        TCL.Name Department, '
      '                        TCL.Code Zone, '
      '                        ct.Modified '
      '                        from contacttime ct '
      '                            Inner Join Contacts C On '
      
        '                                C.Contacts_Key = ct.Contacts_Key' +
        ' '
      
        '                            Left Join TimeClockLocations TCL On ' +
        '                              ct.TimeClockLocations_Key = TCL.Ti' +
        'meClockLocations_Key '
      '                        where ct.breaks is not null '
      
        '                              and ct.logout_datetime is not null' +
        ' '
      '                              and (C.Contact_Type = 0 '
      '                              or C.Contact_Type = 1) )'
      '   Union All '
      ''
      '        select '
      '           Contact_Type, '
      '           Name, '
      '           Payroll_ID, '
      '           Login_DateTime, '
      '           Logout_DateTime, '
      '           Breaks,'
      '           (tt) * 24 * 60 * 60 Seconds_Worked, '
      '           (tt) * 24 * 60 Minutes_Worked, '
      '           (tt) * 24 Hours_Worked, '
      '           (tt) Days_Worked, '
      '           Department, '
      '           Zone, '
      '           Modified '
      
        '           from (select (ct.logout_datetime - ct.login_datetime)' +
        ' tt, '
      '                        C.Contact_Type, '
      '                        C.Name, '
      '                        C.Payroll_ID, '
      '                        ct.Login_DateTime, '
      '                        ct.Logout_DateTime, '
      '                        ct.Breaks,'
      '                        TCL.Name Department, '
      '                        TCL.Code Zone, '
      '                        ct.Modified '
      '                        from contacttime ct '
      '                            Inner Join Contacts C On '
      
        '                                C.Contacts_Key = ct.Contacts_Key' +
        ' '
      
        '                            Left Join TimeClockLocations TCL On ' +
        '                                ct.TimeClockLocations_Key = TCL.' +
        'TimeClockLocations_Key '
      '                        where ct.breaks is null '
      
        '                              and ct.logout_datetime is not null' +
        ' '
      '                              and (C.Contact_Type = 0 '
      '                              or C.Contact_Type = 1) )'
      'Order By 11, 2;')
    Left = 120
    Top = 504
  end
  object ravWages: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrWages
    Left = 232
    Top = 504
  end
  object qrPatronCount: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'ArcBill.Billed_Location Location,'
      #9'ArcBill.Sales_Type,'
      #9'Sum(ArcBill.Patron_Count) Patron_Count'
      'From'
      #9'Security Left Join ArcBill on'
      #9#9'Security.Security_Ref = ArcBill.Security_Ref'
      'Where'
      #9'Security.Time_Stamp >= '#39'1/1/2004'#39' and'
      #9'Security.Time_Stamp < '#39'1/2/2004'#39' and'
      #9'Security.Security_Event = '#39'Billed By'#39
      'Group By'
      #9'ArcBill.Billed_Location,'
      #9'ArcBill.Sales_Type'
      'Having'
      #9'Count(Patron_Count) > 0'
      'Order By'
      #9'ArcBill.Billed_Location,'
      #9'ArcBill.Sales_Type')
    Filtered = True
    OnFilterRecord = qrPatronCountFilterRecord
    Left = 424
    Top = 8
  end
  object ravPatronCount: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrPatronCount
    Left = 520
    Top = 8
  end
  object qrAveSummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    AfterScroll = qrAveSummaryAfterScroll
    SQL.Strings = (
      'Select'
      #9'ArcBill.ArcBill_Key,'
      #9'ArcBill.Billed_Location Location,'
      #9'CategoryGroups.Name Category_Group,'
      #9'ArcBill.Sales_Type,'
      #9'Max(ArcBill.Total - ArcBill.Discount) Bill_Total,'
      #9'Sum(Archive.Price) Orders_Total,'
      #9'Max(ArcBill.Patron_Count) Patron_Count'
      'From'
      #9'Security Left Join ArcBill on'
      #9#9'Security.Security_Ref = ArcBill.Security_Ref'
      #9'Left Join Archive on'
      #9#9'ArcBill.ArcBill_Key = Archive.ArcBill_Key'
      #9'Left Join ArcCategories on'
      #9#9'Archive.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'1/1/2004'#39' and'
      #9'Security.Time_Stamp < '#39'1/2/2004'#39' and'
      #9'Security.Security_Event = '#39'Billed By'#39
      'Group By'
      #9'ArcBill.Billed_Location,'
      #9'ArcBill.ArcBill_Key,'
      #9'CategoryGroups.Name,'
      #9'ArcBill.Sales_Type'
      'Having'
      #9'Count (Archive.Archive_Key) > 0'
      'Order By'
      #9'ArcBill.Billed_Location,'
      #9'CategoryGroups.Name,'
      #9'ArcBill.Sales_Type')
    Left = 416
    Top = 56
  end
  object ravAveSummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrAveSummary
    Left = 520
    Top = 56
  end
  object qrAveCount: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Sum(ArcBill.Patron_Count) Patron_Count'
      'From'
      #9'Security Left Join ArcBill on'
      #9#9'Security.Security_Ref = ArcBill.Security_Ref'
      'Where'
      #9'Security.Time_Stamp >= '#39'5/5/2003'#39' and'
      #9'Security.Time_Stamp < '#39'5/6/2003'#39' and'
      #9'Security.Security_Event = '#39'Billed By'#39' and'
      #9'ArcBill.Sales_Type = :Sales_Type'
      '')
    Left = 416
    Top = 104
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'Sales_Type'
        ParamType = ptUnknown
      end>
  end
  object ravAveCount: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrAveCount
    Left = 520
    Top = 104
  end
  object qrDiscountSummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Count(ArcBill.ArcBill_Key) Bill_Count,'
      #9'Sum(Archive.Discount) Discount_Total'
      'From'
      #9'Security Left Join ArcBill On'
      #9#9'Security.Security_Ref = ArcBill.Security_Ref'
      #9'Left Join Archive On'
      #9#9'ArcBill.ArcBill_Key = Archive.ArcBill_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'11/5/2003'#39' and'
      #9'Security.Time_Stamp < '#39'11/7/2003'#39' and'
      #9'Security.Security_Event = '#39'Discounted By'#39
      'Group By'
      #9'ArcBill.ArcBill_Key')
    Left = 416
    Top = 152
  end
  object ravDiscountSummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDiscountSummary
    Left = 528
    Top = 152
  end
  object qrAdjustSummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Archive.Order_Type,'
      '                Sum(Archive.Qty) Item_Count,'
      #9'Sum(Archive.Price - Archive.Price_Level0) Total'
      'From'
      #9'Security Left Join Archive On'
      #9#9'Security.Security_Ref = Archive.Security_Ref'
      'Where'
      #9'Security.Time_Stamp >= '#39'5/5/2003'#39' and'
      #9'Security.Time_Stamp < '#39'5/6/2003'#39' and'
      #9'Security.Security_Event = '#39'Ordered By'#39' and'
      #9'Archive.Price <> Archive.Price_Level0'
      'Group By'
      #9'Archive.Order_Type')
    Left = 416
    Top = 200
  end
  object ravAdjustSummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrAdjustSummary
    Left = 520
    Top = 200
  end
  object qrConsumption: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select '
      ''
      'Archive.Menu_Name Group_Name,'
      'Archive.Course_Name,'
      'Cast(Archive.Item_Name as VarChar(50)) Item_Name,'
      'Archive.Size_Name,'
      
        'Cast(Extract (Minute From Security.Time_Stamp) / 30 as Integer) ' +
        'Order_Half_Hour,'
      'Extract (Minute From Security.Time_Stamp) Order_Minute,'
      'Extract (Hour From Security.Time_Stamp) Order_Hour,'
      'Extract (Day From Security.Time_Stamp) Order_Day,'
      'Extract (Month From Security.Time_Stamp) Order_Month,'
      'Extract (Year From Security.Time_Stamp) Order_Year,'
      'Cast(Security.Time_Stamp As Date) Order_Date,'
      ''
      'Cast(Cast('#39'12/30/1899'#39' AS TIMESTAMP) + '
      
        'Cast(((Extract (Minute From Security.Time_Stamp) / 30) * 30 * 60' +
        ') + '
      
        '(Extract (Hour From Security.Time_Stamp) * 60 * 60) As Double Pr' +
        'ecision) / 86400  as Time) Start_Time,'
      ''
      'Cast(Cast('#39'12/30/1899'#39' AS TIMESTAMP) + '
      
        'Cast(((Extract (Minute From Security.Time_Stamp) / 30) * 30 * 60' +
        ') + '
      
        '(Extract (Hour From Security.Time_Stamp) * 60 * 60) + 1800 As Do' +
        'uble Precision) / 86400  as Time) End_Time,'
      ''
      'Archive.Qty Item_Count,'
      'Cast((Archive.Price + Archive.Discount) as Numeric(15,4)) Price,'
      
        'Cast(((Archive.Price + Archive.Discount) * 100 / (Archive.GST_Pe' +
        'rcent + 100)) as Numeric(15,4)) PriceExc,'
      'Cast((Archive.Cost) as Numeric(15,4)) Cost,'
      #39#39' as Code'
      ''
      'From '
      ''
      
        'Security Left Join Archive on Security.Security_Ref = Archive.Se' +
        'curity_Ref '
      ''
      'Where (Archive.Order_Type = 3 or Archive.Order_Type = 0) and '
      
        'Security.Time_Stamp >= '#39'1/1/2007'#39' and Security.Time_Stamp < '#39'1/2' +
        '/2007'#39' and '
      'Security.Security_Event = '#39'Ordered By'#39
      ''
      'Order By 10,9,8,7,5,1,2,3,4'
      '')
    Left = 672
    Top = 8
  end
  object ravConsumption: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrConsumption
    Left = 768
    Top = 8
  end
  object qrUserSales: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Contacts.Name,'
      #9'Archive.Menu_Name Group_Name,'
      #9'Archive.Course_Name,'
      #9'Cast(Archive.Item_Name as VarChar(50)) Item_Name,'
      #9'Archive.Size_Name,'
      #9'Sum(Archive.Qty) Item_Count,'
      #9'Sum(Archive.Price) Price,'
      'Cast(0.0 as numeric(15, 4)) PriceExcl,'
      #9'Sum(Archive.Cost) Cost,'
      'Cast(0.0 as numeric(15, 4)) Profit'
      'From'
      #9'Security Left Join Archive on'
      #9#9'Security.Security_Ref = Archive.Security_Ref'
      #9'Left Join Contacts on'
      #9#9'Security.User_Key = Contacts.Contacts_Key'
      'Where'
      #9'(Archive.Order_Type = 3 or'
      #9'Archive.Order_Type = 0) and'
      #9'Security.Time_Stamp >= '#39'10/25/2002'#39' and'
      #9'Security.Time_Stamp < '#39'10/26/2002'#39' and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'Contacts.Name,'
      #9'Archive.Menu_Name,'
      #9'Archive.Course_Name,'
      #9'Archive.Item_Name,'
      #9'Archive.Size_Name'
      'Having'
      #9'Count(Archive.Archive_Key) > 0'
      ''
      'Union All'
      ''
      'Select'
      #9'Contacts.Name,'
      #9'DayArchive.Menu_Name Group_Name,'
      #9'DayArchive.Course_Name,'
      #9'Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,'
      #9'DayArchive.Size_Name,'
      #9'Sum(DayArchive.Qty) Item_Count,'
      #9'Sum(DayArchive.Price) Price,'
      'Cast(0.0 as numeric(15, 4)) PriceExcl,'
      #9'Sum(DayArchive.Cost) Cost,'
      'Cast(0.0 as numeric(15, 4)) Profit'
      'From'
      #9'Security Left Join DayArchive on'
      #9#9'Security.Security_Ref = DayArchive.Security_Ref'
      #9'Left Join Contacts on'
      #9#9'Security.User_Key = Contacts.Contacts_Key'
      'Where'
      #9'(DayArchive.Order_Type = 3 or'
      #9'DayArchive.Order_Type = 0) and'
      #9'Security.Time_Stamp >= '#39'10/25/2002'#39' and'
      #9'Security.Time_Stamp < '#39'10/26/2002'#39' and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'Contacts.Name,'
      #9'DayArchive.Menu_Name,'
      #9'DayArchive.Course_Name,'
      #9'DayArchive.Item_Name,'
      #9'DayArchive.Size_Name'
      'Having'
      #9'Count(DayArchive.Archive_Key) > 0'
      ''
      'Union All'
      ''
      'Select'
      #9'Contacts.Name,'
      #9'Orders.Menu_Name Group_Name,'
      #9'Orders.Course_Name,'
      #9'Orders.Item_Name,'
      #9'Orders.Size_Name,'
      #9'Sum(Orders.Qty) Item_Count,'
      #9'Sum(Orders.Price) Price,'
      'Cast(0.0 as numeric(15, 4)) PriceExcl,'
      #9'Sum(Orders.Cost) Cost,'
      'Cast(0.0 as numeric(15, 4)) Profit'
      'From'
      #9'Security Left Join Orders on'
      #9#9'Security.Security_Ref = Orders.Security_Ref'
      #9'Left Join Contacts on'
      #9#9'Security.User_Key = Contacts.Contacts_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'10/25/2002'#39' and'
      #9'Security.Time_Stamp < '#39'10/26/2002'#39' and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'Contacts.Name,'
      #9'Orders.Menu_Name,'
      #9'Orders.Course_Name,'
      #9'Orders.Item_Name,'
      #9'Orders.Size_Name'
      'Having'
      #9'Count(Orders.Order_Key) > 0'
      ''
      'Order By'
      #9'1,2,3,4,5')
    Left = 672
    Top = 56
  end
  object ravUserSales: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrUserSales
    Left = 768
    Top = 56
  end
  object qrChronological: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '   Security.Time_Stamp,'
      '   Extract(Day From Security.Time_Stamp) Item_Day,'
      '   Extract(Second From Security.Time_Stamp) Item_Second,'
      '   Cast(Archive.Item_Name as VarChar(50)) Item_Name,'
      '   Archive.Qty,'
      '   Archive.Size_Name,'
      '   Archive.Price,'
      '   Archive.Table_Number,'
      '   Cast(Archive.Tab_Name as VarChar(32)) Tab_Name,'
      '   Security.From_Val User_Name,'
      '   Security.Terminal_Name,'
      '   Archive.Archive_Key Table_Key'
      'From'
      '   Security Left Join Archive on'
      '   Security.Security_Ref = Archive.Security_Ref'
      'Where'
      '   Security.Time_Stamp >= '#39'4/4/2003'#39' and'
      '   Security.Time_Stamp < '#39'4/5/2003'#39' and'
      '   Security.Security_Event = '#39'Ordered By'#39
      ''
      'Union All'
      ''
      'Select'
      '   Security.Time_Stamp,'
      '   Extract(Day From Security.Time_Stamp) Item_Day,'
      '   Extract(Second From Security.Time_Stamp) Item_Second,'
      '   Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,'
      '   DayArchive.Qty,'
      '   DayArchive.Size_Name,'
      '   DayArchive.Price,'
      '   DayArchive.Table_Number,'
      '   Cast(DayArchive.Tab_Name as VarChar(32)) Tab_Name,'
      '   Security.From_Val User_Name,'
      '   Security.Terminal_Name,'
      '   DayArchive.Archive_Key Table_Key'
      'From'
      '   Security Right Join DayArchive on'
      '   Security.Security_Ref = DayArchive.Security_Ref'
      'Where'
      '   Security.Time_Stamp >= '#39'4/4/2003'#39' and'
      '   Security.Time_Stamp < '#39'4/5/2003'#39' and'
      '   Security.Security_Event = '#39'Ordered By'#39
      ''
      'Union All'
      ''
      'Select'
      '   Security.Time_Stamp,'
      '   Extract(Day From Security.Time_Stamp) Item_Day,'
      '   Extract(Second From Security.Time_Stamp) Item_Second,'
      '   Cast(Orders.Item_Name as VarChar(50)) Item_Name,'
      '   Orders.Qty,'
      '   Orders.Size_Name,'
      '   Orders.Price,'
      '   Orders.Table_Number,'
      '   Cast(Orders.Tab_Name as VarChar(32)) Tab_Name,'
      '   Security.From_Val User_Name,'
      '   Security.Terminal_Name,'
      '   Orders.Order_Key Table_Key'
      'From'
      '   Security Inner Join Orders on'
      '   Security.Security_Ref = Orders.Security_Ref'
      'Where'
      '   Security.Time_Stamp >= '#39'4/4/2003'#39' and'
      '   Security.Time_Stamp < '#39'4/5/2003'#39' and'
      '   Security.Security_Event = '#39'Ordered By'#39
      ''
      'Order By'
      '   1,4,5')
    Left = 672
    Top = 168
  end
  object ravChronological: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrChronological
    Left = 768
    Top = 168
  end
  object qrTurnAround: TIBQuery
    Database = dmChefMateData.dbChefMate
    Transaction = ChefMateTrans
    SQL.Strings = (
      'Select'
      #9'Extract (Minute From Order_Sale_Start_Time) Sale_Minute,'
      #9'Extract (Hour From Order_Sale_Start_Time) Sale_Hour,'
      #9'Extract (Day From Order_Sale_Start_Time) Sale_Day,'
      #9'Extract (Month From Order_Sale_Start_Time) Slae_Month,'
      #9'Extract (Year From Order_Sale_Start_Time) Sale_Year,'
      ''
      #9'CAST('#39'12/30/1899'#39' AS TIMESTAMP) + '
      
        #9'(Cast((Extract(Hour From Order_Sale_Start_Time)) As Double Prec' +
        'ision) / 24 + .0000000001) Start_Time,'
      ''
      #9'CAST('#39'12/30/1899'#39' AS TIMESTAMP) + '
      
        #9'(Cast((Extract(Hour From Order_Sale_Start_Time) + 1) As Double ' +
        'Precision) / 24 + .0000000001) End_Time,'
      ''
      #9'Order_Sale_Start_Time,'
      #9'Order_Sale_Finish_Time,'
      #9'Arrival_Time,'
      #9'Bump_Time,'
      ''
      
        #9'CAST('#39'12/30/1899'#39' AS TIMESTAMP) + (Order_Sale_Finish_Time - Ord' +
        'er_Sale_Start_Time) Sale_Time,'
      
        #9'CAST('#39'12/30/1899'#39' AS TIMESTAMP) + (Bump_Time - Arrival_Time) Ma' +
        'ke_Time,'
      
        #9'CAST('#39'12/30/1899'#39' AS TIMESTAMP) + (Bump_Time - Order_Sale_Start' +
        '_Time) Process_Time '
      'From '
      #9'Orders Inner Join OrderTimesView '
      #9'On Orders.Order_Key = OrderTimesView.Order_Key '
      'Where '
      #9'Order_Sale_Start_Time >= :StartTime and '
      #9'Order_Sale_Start_Time < :EndTime and '
      #9'Order_Sale_Start_Time Is Not Null And '
      #9'Order_Sale_Finish_Time Is Not Null And '
      #9'Arrival_Time Is Not Null And '
      #9'Bump_Time Is Not Null '
      'Order By'
      #9'Order_Sale_Start_Time')
    Left = 416
    Top = 248
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
  object ravTurnAround: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrTurnAround
    Left = 520
    Top = 248
  end
  object qrBillPayments: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    AfterScroll = qrBillPaymentsAfterScroll
    SQL.Strings = (
      'Select '
      #9'ArcBill.ArcBill_Key,'
      #9'Security.Time_Stamp,'
      #9'ArcBill.Invoice_Number,'
      #9'ArcBill.Total,'
      #9'ArcBill.Patron_Count,'
      #9'Security.Terminal_Name,'
      #9'Security.From_Val Staff_Name,'
      #9'ArcBillPay.Pay_Type,'
      #9'ArcBillPay.SubTotal,'
      #9'ArcBillPay.Note '
      'From '
      #9'ArcBill Inner Join Security On '
      #9#9'ArcBill.Security_Ref = Security.Security_Ref '
      #9'Left Join ArcBillPay On '
      #9#9'ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key '
      'Where '
      #9'Security.Time_Stamp >= '#39'9/9/2003'#39' And '
      #9'Security.Time_Stamp < '#39'9/10/2003'#39' And '
      #9'Security.Security_Event = '#39'Billed By'#39' ')
    Left = 672
    Top = 216
  end
  object ravBillPayments: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrBillPayments
    Left = 768
    Top = 216
  end
  object qrBillDetails: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      #9#9'Select '
      #9#9#9'Cast(Archive.Size_Name As VarChar(25)) Size_Name,'
      #9#9#9'Cast(Archive.Item_Name As VarChar(50)) Item_Name,'
      #9#9#9'Archive.Price + Archive.Discount + Archive.Redeemed Price,'
      
        '                                                Archive.Discount' +
        ', '
      
        '                                                (Archive.Price +' +
        ' Archive.Discount) - Archive.Discount DiscPrice,'
      #9#9#9'Security.Time_Stamp,'
      #9#9#9'Security.Security_Event,'
      #9#9#9'Contacts.Name '
      #9#9'From '
      #9#9#9'Archive Left Join Security On '
      #9#9#9#9'Archive.Security_Ref = Security.Security_Ref '
      #9#9#9'Left Join Contacts On '
      #9#9#9#9'Security.User_Key = Contacts.Contacts_Key '
      #9#9'Where '
      #9#9#9'Archive.ArcBill_Key = :ArcBill_Key '
      ''
      #9#9'Union All '
      ''
      #9#9'Select '
      #9#9#9'Cast('#39#39' As VarChar(25)) Size_Name,'
      #9#9#9'Cast(ArcSurcharge.Pay_Type As VarChar(50)) Item_Name,'
      #9#9#9'Cast(ArcSurcharge.SubTotal As Numeric(15, 4)) Price,'
      
        '                                                Cast(0.00 as num' +
        'eric(15,4)) Discount, '
      
        '                                                Cast(ArcSurcharg' +
        'e.SubTotal As Numeric(15, 4)) DiscPrice,'
      #9#9#9'Security.Time_Stamp,'
      #9#9#9'Cast('#39'Ordered By'#39' As VarChar(50)),'
      #9#9#9'Contacts.Name '
      #9#9'From '
      #9#9#9'ArcSurcharge Left Join ArcBill On '
      #9#9#9#9'ArcSurcharge.ArcBill_Key = ArcBill.ArcBill_Key '
      #9#9#9'Left Join Security On '
      #9#9#9#9'ArcBill.Security_Ref = Security.Security_Ref '
      #9#9#9'Left Join Contacts On '
      #9#9#9#9'Security.User_Key = Contacts.Contacts_Key '
      #9#9'Where '
      #9#9#9'ArcSurcharge.ArcBill_Key = :ArcBill_Key '
      ''
      #9#9'Order By '
      #9#9#9'4,'
      #9#9#9'2,'
      #9#9#9'1,'
      #9#9#9'5 Desc')
    Left = 672
    Top = 264
    ParamData = <
      item
        DataType = ftUnknown
        Name = 'ArcBill_Key'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'ArcBill_Key'
        ParamType = ptUnknown
      end>
  end
  object ravBillDetails: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrBillDetails
    Left = 768
    Top = 264
  end
  object qrTabSummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '   Tab.Tab_Key,'
      '   Tab.Tab_Name,'
      '   0-Tab.Credit Credit,'
      '   Extract(Day From Orders.Time_Stamp) Trans_Day,'
      '   Extract(Month From Orders.Time_Stamp) Trans_Month,'
      '   Extract(Year From Orders.Time_Stamp) Trans_Year,'
      '   Orders.Time_Stamp,'
      '   Sum(Orders.Qty) Qty,'
      '   Orders.Course_Name,'
      '   Orders.Item_Name,'
      '   Orders.Size_Name,'
      '   Sum(Orders.Price + Orders.Discount) Total_Price'
      'From'
      '   Tab Left Join Orders On'
      '      Tab.Tab_Key = Orders.Tab_Key'
      'Where'
      '   (Tab.Tab_Type = 0 Or'
      '   Tab.Tab_Type = 1 Or'
      '   Tab.Tab_Type = 2) And'
      '   (Orders.Order_Type = 3 Or'
      '    Orders.Order_Type = 0)'
      'Group By'
      '   Tab.Tab_Key,'
      '   Tab.Tab_Name,'
      '   Tab.Credit,'
      '   Orders.Time_Stamp,'
      '   Orders.Course_Name,'
      '   Orders.Item_Name,'
      '   Orders.Size_Name'
      'Order By'
      '   Tab.Tab_Name,'
      '   Tab.Tab_Key,'
      '   Orders.Time_Stamp,'
      '   Orders.Course_Name,'
      '   Orders.Item_Name,'
      '   Orders.Size_Name')
    Left = 672
    Top = 312
  end
  object ravTabSummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrTabSummary
    Left = 768
    Top = 312
  end
  object qrTabDetails: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '   Tab_Name,'
      '   ID_Type,'
      '   ID_Number,'
      '   ID_Expire ,'
      '   Tab_Type,'
      '   Tab_Permanent '
      'From '
      '   Tab '
      'Where '
      '   (Tab.Tab_Type = 0 Or '
      '   Tab.Tab_Type = 1 Or '
      '   Tab.Tab_Type = 2) '
      'Order By '
      '   Tab.Tab_Name')
    Left = 672
    Top = 360
  end
  object ravTabDetails: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrTabDetails
    Left = 768
    Top = 360
  end
  object qrDiscounts: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Security.Time_Stamp,'
      #9'Security.Terminal_Name,'
      #9'Extract(Day From Security.Time_Stamp) Bill_Day,'
      #9'Extract(Month From Security.Time_Stamp) Bill_Month,'
      #9'Extract(Year From Security.Time_Stamp) Bill_Year,'
      #9'Security.Note,'
      #9'Contacts.Name,'
      #9'ArcBill.Invoice_Number,'
      #9'ArcBill.Total,'
      #9'Archive.Archive_Key,'
      #9'Archive.Discount,'
      #9'ArcBill.ArcBill_Key,'
      #9'Cast(Archive.Size_Name As VarChar(30)) Size_Name,'
      #9'Cast(Archive.Item_Name As VarChar(50)) Item_Name,'
      #9'Archive.Price Price,'
      #9'Archive.Price + Archive.Discount Price_Paid,'
      #9'ArcOrderDiscounts.Discounted_Value,'
      #9'ArcOrderDiscounts.Description Discount_Name,'
      #9'Menu.Menu_Type,'
      #9'cast(Archive.Order_Location as Varchar(25)) Order_Location,'
      #9'Archive.Cost,'
      #9'Archive.Qty Quantity,'
      #9'ArcCategories.Category,'
      #9'CategoryGroups.Name CategoryGroup'
      'From'
      #9'Security Left Join ArcBill On'
      #9#9'Security.Security_Ref = ArcBill.Security_Ref'
      #9'Left Join Contacts On'
      #9#9'Security.User_Key = Contacts.Contacts_Key'
      #9'Left Join Archive On'
      #9#9'ArcBill.ArcBill_Key = Archive.ArcBill_Key'
      #9'Left Join Menu On'
      #9#9'Archive.Menu_Name = Menu.Menu_Name'
      #9'Left Join ArcCategories on'
      #9#9'Archive.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      #9'Left Join ArcOrderDiscounts on'
      #9#9'Archive.Archive_Key = ArcOrderDiscounts.Archive_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'7/1/2010'#39' and'
      #9'Security.Time_Stamp < '#39'8/2/2010'#39' and'
      #9'Security.Security_Event = '#39'Discounted By'#39' And'
      #9'ArcBill.Invoice_Number Is Not Null and'
      #9'Archive.Archive_Key Is Not Null and'
      #9'Archive.Discount != 0'
      ''
      'Union All'
      ''
      'Select'
      #9'Security.Time_Stamp,'
      #9'Security.Terminal_Name,'
      #9'Extract(Day From Security.Time_Stamp) Bill_Day,'
      #9'Extract(Month From Security.Time_Stamp) Bill_Month,'
      #9'Extract(Year From Security.Time_Stamp) Bill_Year,'
      #9'Security.Note,'
      #9'Contacts.Name,'
      #9'DayArcBill.Invoice_Number,'
      #9'DayArcBill.Total,'
      #9'DayArchive.Archive_Key,'
      #9'DayArchive.Discount,'
      #9'DayArcBill.ArcBill_Key,'
      #9'Cast(DayArchive.Size_Name As VarChar(30)) Size_Name,'
      #9'Cast(DayArchive.Item_Name As VarChar(50)) Item_Name,'
      #9'DayArchive.Price Price,'
      #9'DayArchive.Price + DayArchive.Discount Price_Paid,'
      #9'DayArcOrderDiscounts.Discounted_Value,'
      #9'DayArcOrderDiscounts.Description Discount_Name,'
      #9'Menu.Menu_Type,'
      #9'cast(DayArchive.Order_Location as Varchar(25)) Order_Location,'
      #9'DayArchive.Cost,'
      #9'DayArchive.Qty Quantity,'
      #9'ArcCategories.Category,'
      #9'CategoryGroups.Name CategoryGroup'
      'From'
      #9'Security Left Join DayArcBill On'
      #9#9'Security.Security_Ref = DayArcBill.Security_Ref'
      #9'Left Join Contacts On'
      #9#9'Security.User_Key = Contacts.Contacts_Key'
      #9'Left Join DayArchive On'
      #9#9'DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key'
      #9'Left Join Menu On'
      #9#9'DayArchive.Menu_Name = Menu.Menu_Name'
      #9'Left Join ArcCategories on'
      #9#9'DayArchive.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      #9'Left Join DayArcOrderDiscounts on'
      #9#9'DayArchive.Archive_Key = DayArcOrderDiscounts.Archive_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'7/1/2010'#39' and'
      #9'Security.Time_Stamp < '#39'8/2/2010'#39' and'
      #9'Security.Security_Event = '#39'Discounted By'#39' And'
      #9'DayArcBill.Invoice_Number Is Not Null and'
      #9'DayArchive.Archive_Key Is Not Null and'
      #9'DayArchive.Discount != 0')
    Left = 672
    Top = 408
  end
  object ravDiscounts: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDiscounts
    Left = 768
    Top = 408
  end
  object qrCredits: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Security.Terminal_Name,'
      #9'Security.Note,'
      #9'Archive.Size_Name,'
      #9'Cast(Archive.Item_Name As Varchar(50)) Item_Name,'
      #9'Archive.Price,'
      #9'Archive.Price Total_Price,'
      #9'Extract(Day From Security.Time_Stamp) Credit_Day,'
      #9'Extract(Month From Security.Time_Stamp) Credit_Month,'
      #9'Extract(Year From Security.Time_Stamp) Credit_Year,'
      #9'Security.Time_Stamp,'
      #9'Contacts.Name User_Name'
      'From'
      #9'Security Inner Join Archive On'
      #9#9'Security.Security_Ref = Archive.Security_Ref'
      #9'Inner Join Contacts On'
      #9#9'Contacts.Contacts_Key = Security.User_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'1/1/2004'#39' and'
      #9'Security.Time_Stamp < '#39'3/3/2004'#39' and'
      #9'Security.Security_Event = '#39'Credit'#39
      ''
      'Union All'
      ''
      'Select'
      #9'Security.Terminal_Name,'
      #9'Security.Note,'
      #9'DayArchive.Size_Name,'
      #9'Cast(DayArchive.Item_Name As Varchar(50)) Item_Name,'
      #9'DayArchive.Price,'
      #9'DayArchive.Price Total_Price,'
      #9'Extract(Day From Security.Time_Stamp) Credit_Day,'
      #9'Extract(Month From Security.Time_Stamp) Credit_Month,'
      #9'Extract(Year From Security.Time_Stamp) Credit_Year,'
      #9'Security.Time_Stamp,'
      #9'Contacts.Name User_Name'
      'From'
      #9'Security Inner Join DayArchive On'
      #9#9'Security.Security_Ref = DayArchive.Security_Ref'
      #9'Inner Join Contacts On'
      #9#9'Contacts.Contacts_Key = Security.User_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'1/1/2004'#39' and'
      #9'Security.Time_Stamp < '#39'3/3/2004'#39' and'
      #9'Security.Security_Event = '#39'Credit'#39
      ''
      'Order By'
      #9'11, 4')
    Left = 672
    Top = 600
  end
  object ravCredits: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCredits
    Left = 768
    Top = 600
  end
  object qrLoyalty: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select '
      '   Name,'
      '   Member_Number,'
      '   Total_Spent Points_Earnt,'
      '   Earnt_Points,'
      '   Loaded_Points,'
      '   Cast(0 As Numeric(15,4)) Points_Redeemed,'
      '   Total_Spent Points_Remaining,'
      '   DateOfBirth Birthday'
      'From '
      '   Contacts '
      'Where '
      '   Contact_Type in (2,4) '
      'Order By'
      '   Name')
    Left = 416
    Top = 320
  end
  object ravLoyalty: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrLoyalty
    Left = 520
    Top = 320
  end
  object qrCancelsBill: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    AfterScroll = qrCancelsBillAfterScroll
    SQL.Strings = (
      'Select Distinct'
      #9'ArcBill.ArcBill_Key,'
      #9'ArcBill.Invoice_Number,'
      #9'SecBill.Terminal_Name,'
      #9'SecBill.Time_Stamp,'
      #9'UsersBill.Name'
      'From'
      #9'Security SecurityArc Left Join Archive On'
      #9#9'SecurityArc.Security_Ref = Archive.Security_Ref'
      #9'Left Join ArcBill On'
      #9#9'ArcBill.ArcBill_Key = Archive.ArcBill_Key'
      #9'Left Join Security SecBill On'
      #9#9'SecBill.Security_Ref = ArcBill.Security_Ref'
      #9'Left Join Contacts UsersBill On'
      #9#9'SecBill.User_Key = UsersBill.Contacts_Key'
      'Where'
      'ArcBill.ArcBill_Key = 253440 and'
      #9' SecBill.Time_Stamp >= '#39'1/1/2004'#39' and'
      #9' SecBill.Time_Stamp < '#39'2/1/2004'#39' and'
      #9' SecurityArc.Security_Event = '#39'Cancel'#39
      'Order By'
      #9'SecBill.Time_Stamp,'
      #9'ArcBill.Invoice_Number')
    Left = 672
    Top = 504
  end
  object ravCancelsBill: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCancelsBill
    Left = 768
    Top = 504
  end
  object qrAdjustments: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Security.Security_Event,'
      #9'Security.Terminal_Name,'
      #9'Security.Note,'
      #9'Archive.Size_Name,'
      #9'Cast(Archive.Item_Name As Varchar(50)) Item_Name,'
      #9'Archive.Price,'
      #9'Archive.Price Total_Price,'
      #9'Extract(Day From Security.Time_Stamp) Adjust_Day,'
      #9'Extract(Month From Security.Time_Stamp) Adjust_Month,'
      #9'Extract(Year From Security.Time_Stamp) Adjust_Year,'
      #9'Security.Time_Stamp Time_Stamp1,'
      #9'Security.Time_Stamp Time_Stamp2,'
      #9'Contacts.Name User_Name'
      'From'
      #9'Security Inner Join Archive On'
      #9#9'Security.Security_Ref = Archive.Security_Ref'
      #9'Inner Join Contacts On'
      #9#9'Contacts.Contacts_Key = Security.User_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'1/1/2004'#39' and'
      #9'Security.Time_Stamp < '#39'3/1/2004'#39' and'
      #9'Security.Security_Event <> '#39'Ordered By'#39
      ''
      'Union All'
      ''
      'Select'
      #9'Security.Security_Event,'
      #9'Security.Terminal_Name,'
      #9'Security.Note,'
      #9'DayArchive.Size_Name,'
      #9'Cast(DayArchive.Item_Name As Varchar(50)) Item_Name,'
      #9'DayArchive.Price,'
      #9'DayArchive.Price Total_Price,'
      #9'Extract(Day From Security.Time_Stamp) Adjust_Day,'
      #9'Extract(Month From Security.Time_Stamp) Adjust_Month,'
      #9'Extract(Year From Security.Time_Stamp) Adjust_Year,'
      #9'Security.Time_Stamp Time_Stamp1,'
      #9'Security.Time_Stamp Time_Stamp2,'
      #9'Contacts.Name User_Name'
      'From'
      #9'Security Inner Join DayArchive On'
      #9#9'Security.Security_Ref = DayArchive.Security_Ref'
      #9'Inner Join Contacts On'
      #9#9'Contacts.Contacts_Key = Security.User_Key'
      'Where'
      #9'Security.Time_Stamp >= '#39'1/1/2004'#39' and'
      #9'Security.Time_Stamp < '#39'3/1/2004'#39' and'
      #9'Security.Security_Event <> '#39'Ordered By'#39
      ''
      'Order By'
      #9'12, 13, 2, 3')
    Left = 416
    Top = 464
  end
  object ravAdjustments: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrAdjustments
    Left = 520
    Top = 464
  end
  object qrCancelsDet: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Archive.Archive_Key,'
      #9'Archive.ArcBill_Key,'
      #9'Archive.Table_Number,'
      #9'Archive.Seat_Number,'
      #9'Extract(Year From Archive.Time_Stamp) Bill_Year,'
      #9'Extract(Month From Archive.Time_Stamp) Bill_Month,'
      #9'Extract(Day From Archive.Time_Stamp) Bill_Day,'
      #9'Archive.Time_Stamp Order_Time_Stamp,'
      #9'Archive.Tab_Name,'
      #9'Archive.Size_Name,'
      #9'Archive.Item_Name,'
      #9'Archive.Price Price,'
      #9'Security.Security_Event,'
      #9'Security.Time_Stamp Time_Stamp1,'
      #9'Security.Time_Stamp Time_Stamp2,'
      #9'Contacts.Name'
      'From'
      #9'Archive Left Join Security On'
      #9#9'Archive.Security_Ref = Security.Security_Ref'
      #9'Left Join Contacts On'
      #9#9'Contacts.Contacts_Key = Security.User_Key'
      'Where'
      #9'Archive.ArcBill_Key = 253440'
      'Order By'
      #9'Archive.Tab_Name,'
      #9'Archive.Table_Number,'
      #9'Archive.Seat_Number,'
      #9'Archive.Time_Stamp,'
      #9'Archive.Item_Name,'
      #9'Archive.Size_Name,'
      #9'Archive.Archive_Key')
    Left = 672
    Top = 552
  end
  object ravCancelsDet: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCancelsDet
    Left = 768
    Top = 552
  end
  object qrPatronAverages: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '   Security.Time_Stamp,'
      '   Extract (Day From Security.Time_Stamp) Bill_Day,'
      '   Extract (Month From Security.Time_Stamp) Bill_Month,'
      '   Extract (Year From Security.Time_Stamp) Bill_Year,'
      '   ArcBill.Sales_Type,'
      '   ArcBill.Total,'
      '   ArcBill.Patron_Count'
      'From'
      '    Security Left  Join ArcBill On'
      '        Security.Security_Ref = ArcBill.Security_Ref'
      'Where'
      '   Security.Time_Stamp >= '#39'3/1/2003'#39' And'
      '   Security.Time_Stamp < '#39'3/30/2003'#39' And'
      '   Security.Security_Event = '#39'Billed By'#39' And'
      '   ArcBill.Patron_Count <> 0'
      'Order By'
      '    ArcBill.Sales_Type,'
      '    Security.Time_Stamp')
    Left = 672
    Top = 104
  end
  object ravPatronAverages: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrPatronAverages
    Left = 768
    Top = 104
  end
  object ravDebtorInvoiceStatement: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDebtorInvoiceStatement
    Left = 232
    Top = 632
  end
  object qrDebtorInvoiceStatement: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Contacts.Name,'
      #9'Contacts.Mailing_Address,'
      #9'Invoices.Creation_Date,'
      #9'Invoices.Invoice_Number,'
      #9'-(Tab.Credit) Credit,'
      #9'Sum(Orders.Price + Orders.Discount) Price,'
      '                HotelCustomer.Customer_Name,'
      '                HotelCustomer.Room_Number'
      'From'
      #9'Contacts Inner Join Invoices on'
      #9#9'Contacts.Contacts_Key = Invoices.Contacts_Key'
      #9'Inner Join Tab on'
      #9#9'Invoices.Invoice_Key = Tab.Invoice_Key '
      #9'Left Join Orders on'
      #9#9'Tab.Tab_Key = Orders.Tab_Key'
      '                Left Join HotelCustomer on'
      
        '                                Invoices.Invoice_Key = HotelCust' +
        'omer.Invoice_Key'
      'Where'
      #9'Invoices.Closed <> '#39'T'#39
      'Group By'
      #9'Contacts.Name,'
      #9'Contacts.Mailing_Address,'
      #9'Invoices.Invoice_Number,'
      #9'Invoices.Creation_Date,'
      #9'Tab.Credit,'
      '                HotelCustomer.Customer_Name,'
      '                HotelCustomer.Room_Number'
      'Order By'
      #9'1, 2, 3')
    Left = 120
    Top = 632
  end
  object qrDebtorInvoice: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Contacts.Name,'
      #9'Contacts.Mailing_Address,'
      #9'Invoices.Invoice_Number,'
      #9'Invoices.Creation_Date,'
      #9'Invoices.Total_Inc,'
      #9'Invoices.Total_Excl,'
      #9'Orders.Size_Name,'
      #9'Cast(Orders.Item_Name as VarChar(50)) Item_Name,'
      #9'Orders.Qty,'
      #9'Orders.Price + Orders.Discount Price,'
      
        '                Cast(((Orders.Qty * Orders.Price + Orders.Discou' +
        'nt)*100 / (Orders.GST_Percent + 100)) as numeric(15,4)) PriceExc' +
        ','
      #9'Orders.Discount,'
      
        '                Cast((Orders.Price_Adjust) as numeric(15,4)) Pri' +
        'ce_Adjust, '
      #9'Orders.Time_Stamp Ordered_Time,'
      '                HotelCustomer.Customer_Name,'
      '                HotelCustomer.Room_Number'
      'From'
      #9'Contacts Inner Join Invoices on'
      #9#9'Contacts.Contacts_Key = Invoices.Contacts_Key'
      #9'Inner Join Tab on'
      #9#9'Invoices.Invoice_Key = Tab.Invoice_Key'
      #9'Left Join Orders on'
      #9#9'Tab.Tab_Key = Orders.Tab_Key '
      '                Left Join HotelCustomer on'
      
        '                                Invoices.Invoice_Key = HotelCust' +
        'omer.Invoice_Key'
      'Where'
      #9'Invoices.Invoice_Number = 66529'
      ''
      'Union All '
      ''
      'Select '
      #9'Contacts.Name,'
      #9'Contacts.Mailing_Address,'
      #9'Invoices.Invoice_Number,'
      #9'Invoices.Creation_Date,'
      #9'Invoices.Total_Inc,'
      #9'Invoices.Total_Excl,'
      #9'DayArchive.Size_Name,'
      #9'Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,'
      #9'DayArchive.Qty,'
      #9'DayArchive.Price + DayArchive.Discount Price,'
      
        '                Cast(((DayArchive.Qty * DayArchive.Price + DayAr' +
        'chive.Discount)*100 / (DayArchive.GST_Percent + 100)) as numeric' +
        '(15,4)) PriceExc,'
      #9'DayArchive.Discount,'
      
        '                Cast((DayArchive.Price_Adjust) as numeric(15,4))' +
        ' Price_Adjust, '
      #9'DayArchive.Time_Stamp Ordered_Time,'
      '                HotelCustomer.Customer_Name,'
      '                HotelCustomer.Room_Number'
      'From'
      #9'Contacts Inner Join Invoices on'
      #9#9'Contacts.Contacts_Key = Invoices.Contacts_Key '
      #9'Inner Join DayArcBill on'
      #9#9'Invoices.Invoice_Key = DayArcBill.Invoice_Key '
      #9'Left Join DayArchive on'
      #9#9'DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key'
      '                Left Join HotelCustomer on'
      
        '                                Invoices.Invoice_Key = HotelCust' +
        'omer.Invoice_Key'
      'Where'
      #9'Invoices.Invoice_Number = 66529'
      ''
      'Union All'
      ''
      'Select '
      #9'Contacts.Name,'
      #9'Contacts.Mailing_Address,'
      #9'Invoices.Invoice_Number,'
      #9'Invoices.Creation_Date,'
      #9'Invoices.Total_Inc,'
      #9'Invoices.Total_Excl,'
      #9'Archive.Size_Name,'
      #9'Cast(Archive.Item_Name as VarChar(50)) Item_Name,'
      #9'Archive.Qty,'
      #9'Archive.Price + Archive.Discount Price,'
      
        '                Cast(((Archive.Qty * Archive.Price + Archive.Dis' +
        'count)*100 / (Archive.GST_Percent + 100)) as numeric(15,4)) Pric' +
        'eExc,'
      #9'Archive.Discount,'
      
        '                Cast((Archive.Price_Adjust) as numeric(15,4)) Pr' +
        'ice_Adjust, '
      #9'Archive.Time_Stamp Ordered_Time,'
      '                HotelCustomer.Customer_Name,'
      '                HotelCustomer.Room_Number '
      'From '
      #9'Contacts Inner Join Invoices on'
      #9#9'Contacts.Contacts_Key = Invoices.Contacts_Key '
      #9'Inner Join ArcBill on '
      #9#9'Invoices.Invoice_Key = ArcBill.Invoice_Key'
      #9'Left Join Archive on'
      #9#9'ArcBill.ArcBill_Key = Archive.ArcBill_Key'
      '                Left Join HotelCustomer on'
      
        '                                Invoices.Invoice_Key = HotelCust' +
        'omer.Invoice_Key'
      'Where'
      #9'Invoices.Invoice_Number = 66529'
      ''
      'Order By'
      #9'8,'
      #9'7')
    Left = 120
    Top = 584
  end
  object ravDebtorInvoice: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDebtorInvoice
    Left = 232
    Top = 584
  end
  object ravDebtorSummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDebtorSummary
    Left = 232
    Top = 680
  end
  object qrDebtorSummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Contacts.Name,'
      #9'-Tab.Credit Credit,'
      #9'Sum(Orders.Price) Total'
      'From'
      #9'Contacts Inner Join Invoices on'
      #9#9'Contacts.Contacts_Key = Invoices.Contacts_Key'
      #9'Inner Join Tab on'
      #9#9'Invoices.Invoice_Key = Tab.Invoice_Key'
      #9'Left Join Orders on'
      #9#9'Tab.Tab_Key = Orders.Tab_Key'
      'Where'
      #9'Invoices.Closed <> '#39'T'#39
      'Group By'
      #9'Contacts.Name,'
      #9'Tab.Tab_Key,'
      #9'Tab.Credit'
      'Order By'
      #9'1')
    Left = 120
    Top = 680
  end
  object qrSavedSales: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Tab.Tab_Type,'
      #9'Tab.Tab_Name,'
      #9'Contacts.Name Member_Name,'
      #9'Tables.Table_Name,'
      #9'Tables.Table_Number,'
      #9'Sum(Orders.Price + Orders.Discount) Total'
      'From'
      #9'Orders Left Join Tab On'
      #9'Orders.Tab_Key = Tab.Tab_Key'
      #9'Left join Seat On Tab.Tab_Key = Seat.Tab_Key'
      #9'Left Join Tables on Seat.Table_Key = Tables.Table_Key'
      #9'left join Contacts on Contacts.Contacts_Key = Tab.Contacts_Key'
      'Group By'
      #9'Tab.Tab_Type,'
      #9'Tab.Tab_Key,'
      #9'Tables.Table_Number,'
      '        Tables.Table_Name,'
      #9'Tab.Tab_Name,'
      #9'Contacts.Name'
      'Order By'
      #9'Tab.Tab_Type,'
      #9'Tables.Table_Number,'
      #9'Tables.Table_Name,'
      #9'Tab.Tab_Name')
    Left = 416
    Top = 416
  end
  object ravSavedSales: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSavedSales
    Left = 520
    Top = 416
  end
  object qrCancels: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'CanSec.Terminal_Name,'
      #9'CanSec.Note,'
      #9'Archive.Size_Name,'
      #9'Cast(Archive.Item_Name As Varchar(50)) Item_Name,'
      #9'Archive.Price,'
      #9'Extract(Day From CanSec.Time_Stamp) Credit_Day,'
      #9'Extract(Month From CanSec.Time_Stamp) Credit_Month,'
      #9'Extract(Year From CanSec.Time_Stamp) Credit_Year,'
      #9'CanSec.Time_Stamp Cancel_Time,'
      #9'CanSec.Time_Stamp Cancel_Date,'
      #9'CanContact.Name Cancel_Name,'
      #9'OrdContact.Name Order_Name,'
      #9'OrdSec.Time_Stamp Ordered_Date,'
      #9'ArcBill.Invoice_Number,'
      #9'Archive.Tab_Name,'
      #9'Archive.Table_Number,'
      #9'Archive.Seat_Number'
      'From'
      #9'Security CanSec Inner Join Archive On'
      #9#9'CanSec.Security_Ref = Archive.Security_Ref'
      #9'Inner Join Contacts CanContact On'
      #9#9'CanContact.Contacts_Key = CanSec.User_Key'
      #9'Left Join ArcBill On'
      #9#9'Archive.ArcBill_Key = ArcBill.ArcBill_Key'
      #9'Left Join Security OrdSec On'
      #9#9'OrdSec.Security_Ref = Archive.Security_Ref'
      #9'Left Join Contacts OrdContact On'
      #9#9'OrdContact.Contacts_Key = OrdSec.User_Key'
      'Where'
      #9'CanSec.Security_Event = '#39'Cancel'#39' And'
      #9'OrdSec.Security_Event = '#39'Ordered By'#39' And'
      #9'Archive.Archive_Key Is Not Null'
      ''
      'Union All'
      ''
      'Select'
      #9'CanSec.Terminal_Name,'
      #9'CanSec.Note,'
      #9'DayArchive.Size_Name,'
      #9'Cast(DayArchive.Item_Name As Varchar(50)) Item_Name,'
      #9'DayArchive.Price,'
      #9'Extract(Day From CanSec.Time_Stamp) Credit_Day,'
      #9'Extract(Month From CanSec.Time_Stamp) Credit_Month,'
      #9'Extract(Year From CanSec.Time_Stamp) Credit_Year,'
      #9'CanSec.Time_Stamp Cancel_Time,'
      #9'CanSec.Time_Stamp Cancel_Date,'
      #9'CanContact.Name Cancel_Name,'
      #9'OrdContact.Name Order_Name,'
      #9'OrdSec.Time_Stamp Ordered_Date,'
      #9'DayArcBill.Invoice_Number,'
      #9'DayArchive.Tab_Name,'
      #9'DayArchive.Table_Number,'
      #9'DayArchive.Seat_Number'
      'From'
      #9'Security CanSec Inner Join DayArchive On'
      #9#9'CanSec.Security_Ref = DayArchive.Security_Ref'
      #9'Inner Join Contacts CanContact On'
      #9#9'CanContact.Contacts_Key = CanSec.User_Key'
      #9'Left Join DayArcBill On'
      #9#9'DayArchive.ArcBill_Key = DayArcBill.ArcBill_Key'
      #9'Left Join Security OrdSec On'
      #9#9'OrdSec.Security_Ref = DayArchive.Security_Ref'
      #9'Left Join Contacts OrdContact On'
      #9#9'OrdContact.Contacts_Key = OrdSec.User_Key'
      'Where'
      #9'CanSec.Security_Event = '#39'Cancel'#39' And'
      #9'OrdSec.Security_Event = '#39'Ordered By'#39' And'
      #9'DayArchive.Archive_Key Is Not Null'
      ''
      'Union All'
      ''
      'Select'
      #9'CanSec.Terminal_Name,'
      #9'CanSec.Note,'
      #9'Orders.Size_Name,'
      #9'Cast(Orders.Item_Name As Varchar(50)) Item_Name,'
      #9'Orders.Price,'
      #9'Extract(Day From CanSec.Time_Stamp) Credit_Day,'
      #9'Extract(Month From CanSec.Time_Stamp) Credit_Month,'
      #9'Extract(Year From CanSec.Time_Stamp) Credit_Year,'
      #9'CanSec.Time_Stamp Cancel_Time,'
      #9'CanSec.Time_Stamp Cancel_Date,'
      #9'CanContact.Name Cancel_Name,'
      #9'OrdContact.Name Order_Name,'
      #9'OrdSec.Time_Stamp Ordered_Date,'
      #9'Cast('#39'[None]'#39' As VarChar(50)) Invoice_Number,'
      #9'Orders.Tab_Name,'
      #9'Orders.Table_Number,'
      #9'Orders.SeatNo'
      'From'
      #9'Security CanSec Left Join Orders On'
      #9#9'CanSec.Security_Ref = Orders.Security_Ref'
      #9'Left Join Contacts CanContact On'
      #9#9'CanContact.Contacts_Key = CanSec.User_Key'
      #9'Left Join Security OrdSec On'
      #9#9'OrdSec.Security_Ref = Orders.Security_Ref'
      #9'Left Join Contacts OrdContact On'
      #9#9'OrdContact.Contacts_Key = OrdSec.User_Key'
      'Where'
      #9'CanSec.Security_Event = '#39'Cancel'#39' And'
      #9'OrdSec.Security_Event = '#39'Ordered By'#39' And'
      #9'Orders.Order_Key Is Not Null'
      'Order By'
      #9'9, 4')
    Left = 672
    Top = 456
  end
  object ravCancels: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCancels
    Left = 768
    Top = 456
  end
  object qrLoyaltyHistory: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'SELECT'
      #9' ARCBILL.BILLED_LOCATION AS ORDER_LOCATION,'
      '                 ARCHIVE.TIME_STAMP,'
      #9' CAST('#39#39' AS VARCHAR(10)) ITEMNAME,'
      #9' CONTACTS.CONTACTS_KEY,'
      #9' CONTACTS.MEMBER_NUMBER,'
      #9' CONTACTS.NAME,'
      #9' CONTACTS.TOTAL_SPENT TOTAL_POINTS,'
      #9' POINTSTRANSACTIONS.ADJUSTMENT_TYPE,'
      #9' SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS,'
      #9' SUM(ARCHIVE.PRICE + ARCHIVE.DISCOUNT) TOTAL_SPENT'
      'FROM'
      #9' POINTSTRANSACTIONS INNER JOIN CONTACTS ON'
      #9#9'  POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY'
      #9' INNER JOIN ARCBILL ON'
      #9#9'  POINTSTRANSACTIONS.INVOICE_NUMBER = ARCBILL.INVOICE_NUMBER'
      #9' LEFT JOIN ARCHIVE ON'
      #9#9'  ARCHIVE.ARCBILL_KEY = ARCBILL.ARCBILL_KEY'
      'GROUP BY'
      #9' ARCBILL.BILLED_LOCATION,'
      #9' CONTACTS.CONTACTS_KEY,'
      #9' CONTACTS.MEMBER_NUMBER,'
      #9' CONTACTS.NAME,'
      #9' CONTACTS.TOTAL_SPENT,'
      #9' POINTSTRANSACTIONS.ADJUSTMENT_TYPE,'
      '                 ARCHIVE.TIME_STAMP')
    Left = 416
    Top = 368
  end
  object ravLoyaltyHistory: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrLoyaltyHistory
    Left = 520
    Top = 368
  end
  object qrPriceAdjust: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'ArcBill.Invoice_Number,'
      #9'SecOrder.Time_Stamp Date_Ordered,'
      #9'SecOrder.Time_Stamp Time_Ordered,'
      #9'ConOrder.Name Ordered_By,'
      #9'Archive.Size_Name,'
      #9'Cast(Archive.Item_Name as Varchar(50)) Item_Name,'
      #9'Archive.Order_Type,'
      #9'Archive.Price,'
      '                Archive.Price as Total,'
      #9'SecAdjust.Time_Stamp Date_Adjusted,'
      #9'SecAdjust.Time_Stamp Time_Adjusted,'
      #9'Extract(Day From SecAdjust.Time_Stamp) Adjust_Day,'
      #9'Extract(Month From SecAdjust.Time_Stamp) Adjust_Month,'
      #9'Extract(Year From SecAdjust.Time_Stamp) Adjust_Year,'
      #9'SecAdjust.Terminal_Name,'
      #9'ConAdjust.Name Adjusted_By,'
      '                Archive.Order_Location'
      ''
      'From'
      
        #9'Security SecOrder Left Join Archive On SecOrder.Security_Ref = ' +
        'Archive.Security_Ref'
      
        #9'Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder' +
        '.User_Key'
      #9'Left Join ArcBill On Archive.ArcBill_Key = ArcBill.ArcBill_Key'
      
        #9'Left Join Security SecAdjust On Archive.Security_Ref = SecAdjus' +
        't.Security_Ref'
      
        #9'Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdj' +
        'ust.User_Key'
      'Where'
      #9'SecOrder.Time_Stamp >= '#39'1/1/2004 5:00'#39' and'
      #9'SecOrder.Time_Stamp < '#39'3/29/2004 5:00'#39' and'
      #9'SecOrder.Security_Event = '#39'Ordered By'#39' and'
      #9'SecAdjust.Security_Event = '#39'Price Adjust'#39' and'
      #9'Archive.Price <> Archive.Price_Level0 and'
      #9'Archive.Order_Type = 0'
      ''
      'Union All'
      ''
      'Select'
      #9'DayArcBill.Invoice_Number,'
      #9'SecOrder.Time_Stamp Date_Ordered,'
      #9'SecOrder.Time_Stamp Time_Ordered,'
      #9'ConOrder.Name Ordered_By,'
      #9'DayArchive.Size_Name,'
      #9'Cast(DayArchive.Item_Name as Varchar(50)) Item_Name,'
      #9'DayArchive.Order_Type,'
      #9'DayArchive.Price,'
      '                DayArchive.Price as Total,'
      #9'SecAdjust.Time_Stamp Date_Adjusted,'
      #9'SecAdjust.Time_Stamp Time_Adjusted,'
      #9'Extract(Day From SecAdjust.Time_Stamp) Adjust_Day,'
      #9'Extract(Month From SecAdjust.Time_Stamp) Adjust_Month,'
      #9'Extract(Year From SecAdjust.Time_Stamp) Adjust_Year,'
      #9'SecAdjust.Terminal_Name,'
      #9'ConAdjust.Name Adjusted_By,'
      '                DayArchive.Order_Location'
      ''
      'From'
      
        #9'Security SecOrder Left Join DayArchive On SecOrder.Security_Ref' +
        ' = DayArchive.Security_Ref'
      
        #9'Left Join Contacts ConOrder On ConOrder.Contacts_Key = SecOrder' +
        '.User_Key'
      
        #9'Left Join DayArcBill On DayArchive.ArcBill_Key = DayArcBill.Arc' +
        'Bill_Key'
      
        #9'Left Join Security SecAdjust On DayArchive.Security_Ref = SecAd' +
        'just.Security_Ref'
      
        #9'Left Join Contacts ConAdjust On ConAdjust.Contacts_Key = SecAdj' +
        'ust.User_Key'
      'Where'
      #9'SecOrder.Time_Stamp >= '#39'1/1/2004 5:00'#39' and'
      #9'SecOrder.Time_Stamp < '#39'3/29/2004 5:00'#39' and'
      #9'SecOrder.Security_Event = '#39'Ordered By'#39' and'
      #9'SecAdjust.Security_Event = '#39'Price Adjust'#39' and'
      #9'DayArchive.Price <> DayArchive.Price_Level0 and'
      #9'DayArchive.Order_Type = 0'
      'Order By'
      #9'1, 2, 5')
    Left = 672
    Top = 648
  end
  object ravPriceAdjust: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrPriceAdjust
    Left = 768
    Top = 648
  end
  object qrCategoryAnalysis: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Archive.Order_Location Location,'
      #9'CategoryGroups.Name Category_Group,'
      #9'ArcCategories.Category,'
      #9'Sum(Archive.Qty) Item_Count,'
      
        #9'Sum((Archive.Price + Archive.Discount) * 100 / (Archive.GST_Per' +
        'cent + 100)) PriceExc,'
      #9'Sum(Archive.Price + Archive.Discount) PriceInc,'
      #9'Sum(Archive.Cost) Cost,'
      
        #9'Sum((Archive.Price + Archive.Discount) * 100 / (Archive.GST_Per' +
        'cent + 100)) - Sum(Archive.Cost) Profit'
      'From'
      #9'Security Left Join Archive on'
      #9#9'Security.Security_Ref = Archive.Security_Ref'
      #9'Left Join ArcCategories on'
      #9#9'Archive.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where'
      #9'(Archive.Order_Type = 3 or'
      #9'Archive.Order_Type = 0) and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'Archive.Order_Location,'
      #9'CategoryGroups.Name,'
      #9'ArcCategories.Category'
      ''
      'Union All'
      ''
      'Select'
      #9'DayArchive.Order_Location Location,'
      #9'CategoryGroups.Name Category_Group,'
      #9'ArcCategories.Category,'
      #9'Sum(DayArchive.Qty) Item_Count,'
      
        #9'Sum((DayArchive.Price + DayArchive.Discount) * 100 / (DayArchiv' +
        'e.GST_Percent + 100)) PriceExc,'
      #9'Sum(DayArchive.Price + DayArchive.Discount) PriceInc,'
      #9'Sum(DayArchive.Cost) Cost,'
      
        #9'Sum((DayArchive.Price + DayArchive.Discount) * 100 / (DayArchiv' +
        'e.GST_Percent + 100)) - Sum(DayArchive.Cost) Profit'
      'From'
      #9'Security Left Join DayArchive on'
      #9#9'Security.Security_Ref = DayArchive.Security_Ref'
      #9'Left Join ArcCategories on'
      #9#9'DayArchive.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where'
      #9'(DayArchive.Order_Type = 3 or'
      #9'DayArchive.Order_Type = 0) and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'DayArchive.Order_Location,'
      #9'CategoryGroups.Name,'
      #9'ArcCategories.Category'
      ''
      'Union All'
      ''
      'Select'
      #9'Orders.Order_Location Location,'
      #9'CategoryGroups.Name Category_Group,'
      #9'ArcCategories.Category,'
      #9'Sum(Orders.Qty) Item_Count,'
      
        #9'Sum((Orders.Price + Orders.Discount) * 100 / (Orders.GST_Percen' +
        't + 100)) PriceExc,'
      #9'Sum(Orders.Price + Orders.Discount) PriceInc,'
      #9'Sum(Orders.Cost) Cost,'
      
        #9'Sum((Orders.Price + Orders.Discount) * 100 / (Orders.GST_Percen' +
        't + 100)) - Sum(Orders.Cost) Profit'
      'From'
      #9'Security Left Join Orders on'
      #9#9'Security.Security_Ref = Orders.Security_Ref'
      #9'Left Join ArcCategories on'
      #9#9'Orders.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where'
      #9'(Orders.Order_Type = 3 or'
      #9'Orders.Order_Type = 0) and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'Orders.Order_Location,'
      #9'CategoryGroups.Name,'
      #9'ArcCategories.Category'
      ''
      'Order By'
      #9'1, 2, 3')
    Left = 416
    Top = 568
  end
  object ravCategoryAnalysis: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCategoryAnalysis
    Left = 304
    Top = 552
  end
  object qrCatLocTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Archive.Order_Location Location,'
      #9'Sum(Archive.Price + Archive.Discount) PriceInc'
      'From'
      #9'Security Left Join Archive on'
      #9#9'Security.Security_Ref = Archive.Security_Ref'
      'Where'
      #9'(Archive.Order_Type = 3 or'
      #9'Archive.Order_Type = 0) and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'Archive.Order_Location'
      #9
      'Union All'
      ''
      'Select'
      #9'DayArchive.Order_Location Location,'
      #9'Sum(DayArchive.Price + DayArchive.Discount) PriceInc'
      'From'
      #9'Security Left Join DayArchive on'
      #9#9'Security.Security_Ref = DayArchive.Security_Ref'
      'Where'
      #9'(DayArchive.Order_Type = 3 or'
      #9'DayArchive.Order_Type = 0) and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'DayArchive.Order_Location'
      ''
      'Union All'
      ''
      'Select'
      #9'Orders.Order_Location Location,'
      #9'Sum(Orders.Price + Orders.Discount) PriceInc'
      'From'
      #9'Security Left Join Orders on'
      #9#9'Security.Security_Ref = DayArchive.Security_Ref'
      'Where'
      #9'(Orders.Order_Type = 3 or'
      #9'Orders.Order_Type = 0) and'
      #9'Security.Security_Event = '#39'Ordered By'#39
      'Group By'
      #9'Orders.Order_Location'
      ''
      'Order By'
      #9'1')
    Left = 416
    Top = 664
  end
  object ravCatLocTotal: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCatLocTotal
    Left = 520
    Top = 664
  end
  object qrManualCashDrawer: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Security.Time_Stamp Time_Stamp1,'
      #9'Security.Time_Stamp Time_Stamp2,'
      #9'Extract (Day From Security.Time_Stamp) Drawer_Day,'
      #9'Extract (Month From Security.Time_Stamp) Drawer_Month,'
      #9'Extract (Year From Security.Time_Stamp) Drawer_Year,'
      #9'Contacts.Name User_Name,'
      #9'Security.Terminal_Name'
      'From'
      #9'Security Inner Join Contacts On'
      #9#9'Contacts.Contacts_Key = Security.User_Key'
      'Where'
      #9'Security.Security_Event = '#39'Manually Opened Cash Drawer'#39
      ''
      'Order By'
      #9'1, 2, 3')
    Left = 416
    Top = 512
  end
  object ravManualCashDrawer: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrManualCashDrawer
    Left = 520
    Top = 512
  end
  object qrCategoryBreakdown: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9'Archive.Order_Location Location,'
      #9'CategoryGroups.Name Category_Group,'
      #9'ArcCategories.Category,'
      #9'Sum(Archive.Qty) Item_Count,'
      
        #9'Sum((Archive.Price) * 100 / (Archive.GST_Percent + 100)) PriceE' +
        'xc,'
      #9'Sum(Archive.Price) PriceInc,'
      #9'Sum(Archive.Cost) Cost'
      'From'
      #9'Security Left Join Archive on'
      #9#9'Security.Security_Ref = Archive.Security_Ref'
      #9'Left Join ArcCategory on'
      #9#9'Archive.Archive_Key = ArcCategory.Archive_Key'
      #9'Left Join ArcCategories on'
      #9#9'ArcCategory.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where'
      #9'(Archive.Order_Type = 3 or'
      #9'Archive.Order_Type = 0) and'
      
        #9'(Security.Security_Event = '#39'Ordered By'#39' Or Security.Security_Ev' +
        'ent = '#39'Credit'#39')'
      'Group By'
      #9'Archive.Order_Location,'
      #9'CategoryGroups.Name,'
      #9'ArcCategories.Category'
      'Having'
      #9'Count(Archive.Archive_Key) > 0 And'
      #9'ArcCategories.Category Is Not Null'
      ''
      'Union All'
      ''
      'Select'
      #9'DayArchive.Order_Location Location,'
      #9'CategoryGroups.Name Category_Group,'
      #9'ArcCategories.Category,'
      #9'Sum(DayArchive.Qty) Item_Count,'
      
        #9'Sum((DayArchive.Price) * 100 / (DayArchive.GST_Percent + 100)) ' +
        'PriceExc,'
      #9'Sum(DayArchive.Price) PriceInc,'
      #9'Sum(DayArchive.Cost) Cost'
      'From'
      #9'Security Left Join DayArchive on'
      #9#9'Security.Security_Ref = DayArchive.Security_Ref'
      #9'Left Join DayArcCategory on'
      #9#9'DayArchive.Archive_Key = DayArcCategory.Archive_Key'
      #9'Left Join ArcCategories on'
      #9#9'DayArcCategory.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where'
      #9'(DayArchive.Order_Type = 3 or'
      #9'DayArchive.Order_Type = 0) and'
      
        #9'(Security.Security_Event = '#39'Ordered By'#39' Or Security.Security_Ev' +
        'ent = '#39'Credit'#39')'
      'Group By'
      #9'DayArchive.Order_Location,'
      #9'CategoryGroups.Name,'
      #9'ArcCategories.Category'
      'Having'
      #9'Count(DayArchive.Archive_Key) > 0 And'
      #9'ArcCategories.Category Is Not Null'
      ''
      'Union All'
      ''
      'Select'
      #9'Orders.Order_Location Location,'
      #9'CategoryGroups.Name Category_Group,'
      #9'ArcCategories.Category,'
      #9'Sum(Orders.Qty) Item_Count,'
      
        #9'Sum((Orders.Price) * 100 / (Orders.GST_Percent + 100)) PriceExc' +
        ','
      #9'Sum(Orders.Price) PriceInc,'
      #9'Sum(Orders.Cost) Cost'
      'From'
      #9'Security Left Join Orders on'
      #9#9'Security.Security_Ref = Orders.Security_Ref'
      #9'Left Join OrderCategory on'
      #9#9'Orders.Order_Key = OrderCategory.Order_Key'
      #9'Left Join ArcCategories on'
      #9#9'OrderCategory.Category_Key = ArcCategories.Category_Key'
      #9'Left Join CategoryGroups on'
      
        #9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGrou' +
        'ps_Key'
      'Where'
      
        #9'(Security.Security_Event = '#39'Ordered By'#39' Or Security.Security_Ev' +
        'ent = '#39'Credit'#39')'
      'Group By'
      #9'Orders.Order_Location,'
      #9'CategoryGroups.Name,'
      #9'ArcCategories.Category'
      'Having'
      #9'Count(Orders.Order_Key) > 0 And'
      #9'ArcCategories.Category Is Not Null'
      ''
      'Order By'
      #9'1,2')
    Left = 416
    Top = 616
  end
  object ravCategoryBreakdown: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCategoryBreakdown
    Left = 520
    Top = 616
  end
  object qrReductionStock: TIBQuery
    Database = dmStockData.dbStock
    Transaction = StockTrans
    AfterScroll = qrReductionStockAfterScroll
    SQL.Strings = (
      'Select'
      #9'StockCategory.Stock_Category,'
      #9'StockGroup.Stock_Group,'
      #9'Stock.Code,'
      #9'Stock.Description,'
      #9'Stock.Stocktake_Unit'
      'From'
      #9'Stock  Inner Join StockGroup on'
      #9#9'Stock.Stock_Group_Key = StockGroup.Stock_Group_Key'
      #9'Inner Join StockCategory on'
      
        #9#9'StockGroup.Stock_Category_Key = StockCategory.Stock_Category_K' +
        'ey'
      'Where'
      #9'Stock.Enabled = '#39'T'#39' And'
      #9'(Stock.Deleted = '#39'F'#39' Or Stock.Deleted Is Null)'
      'Order By'
      #9'StockCategory.Sort_Order,'
      #9'StockGroup.Sort_Order,'
      #9'Stock.Description')
    Left = 120
    Top = 112
  end
  object ravReductionStock: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrReductionStock
    Left = 232
    Top = 112
  end
  object qrReductionItems: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select '
      #9'Menu.Menu_Name,'
      #9'Course.Course_Name,'
      #9'Item.Item_Name,'
      #9'ItemSize.Size_Name,'
      #9'ItemSize.Price,'
      #9'Recipe.Stock_Code Code,'
      #9'Recipe.Qty,'
      #9'Recipe.Stock_Location Location '
      'From '
      #9'Menu Left Join Course On '
      #9#9'Menu.Menu_Key = Course.Menu_Key '
      #9'Left Join Item On '
      #9#9'Course.Course_Key = Item.Course_Key '
      #9'Left Join ItemSize On '
      #9#9'Item.Item_Key = ItemSize.Item_Key '
      #9'Left Join Recipe On '
      #9#9'ItemSize.ItemSize_Key = Recipe.ItemSize_Key'
      'Order By '
      #9'Menu.Menu_Name,'
      #9'Course.CAO,'
      #9'Item.IAO,'
      #9'ItemSize.ISAO')
    Left = 120
    Top = 160
  end
  object ravReductionItems: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrReductionItems
    Left = 232
    Top = 160
  end
  object ravLoyaltyDiscProducts: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrLoyaltyDiscProducts
    Left = 328
    Top = 56
  end
  object qrLoyaltyDiscProducts: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9#9#9
      
        '                                               Archive.Menu_Name' +
        ' Group_Name,'
      #9#9#9'Archive.Course_Name,'
      #9#9#9'Cast(Archive.Item_Name as VarChar(50))  Item_Name,'
      #9#9#9'Archive.Size_Name,'
      #9#9#9'Archive.Qty as Item_Count,'
      
        '                                                Archive.Time_Sta' +
        'mp,'
      
        '                                                Security.From_Va' +
        'l as Old_Price,'
      
        '                                                Security.To_Val ' +
        'as New_Price,'
      
        '                                                Security.Note as' +
        ' Discount,'
      #9#9#9'Contacts.Name,'
      
        '                                                Security.Securit' +
        'y_Event '
      #9#9'From '
      
        #9#9#9'Security Left Join Archive on Security.Security_Ref = Archive' +
        '.Security_Ref '
      
        #9#9#9'Inner Join Contacts on Archive.Loyalty_Key = Contacts.Contact' +
        's_Key '
      #9#9'Where '
      #9#9#9'(Archive.Order_Type = 3 or Archive.Order_Type = 0) and '
      #9#9#9'Archive.Time_Stamp >= '#39'01/01/05'#39' and'
      #9#9#9'Archive.Time_Stamp < '#39'12/10/05'#39' and'
      
        #9#9#9'Security.Security_Event  in ( '#39'Member Discount'#39','#39'Location Dis' +
        'count'#39' ) and'
      #9#9#9'Contacts.Contacts_Key <> 0'
      ''
      #9#9'Union All'
      ''
      #9'Select'
      #9#9#9'DayArchive.Menu_Name Group_Name,'
      #9#9#9'DayArchive.Course_Name,'
      #9#9#9'Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,'
      #9#9#9'DayArchive.Size_Name,'
      #9#9#9'DayArchive.Qty as Item_Count,'
      
        '                                                DayArchive.Time_' +
        'Stamp,'
      
        '                                                Security.From_Va' +
        'l as Old_Price,'
      
        '                                                Security.To_Val ' +
        'as New_Price,'
      #9#9#9'Security.Note as Discount,'
      #9#9#9'Contacts.Name,'
      
        '                                               Security.Security' +
        '_Event'
      #9#9'From'
      
        #9#9#9'Security Left Join DayArchive on Security.Security_Ref = DayA' +
        'rchive.Security_Ref'
      
        #9#9#9'Inner Join Contacts on DayArchive.Loyalty_Key = Contacts.Cont' +
        'acts_Key'
      #9#9'Where'
      #9#9#9'(DayArchive.Order_Type = 3 or DayArchive.Order_Type = 0) and'
      #9#9#9'DayArchive.Time_Stamp >= '#39'01/01/05'#39' and'
      #9#9#9'DayArchive.Time_Stamp < '#39'12/10/05'#39' and'
      
        #9#9#9'Security.Security_Event  in ( '#39'Member Discount'#39','#39'Location Dis' +
        'count'#39' ) and'
      #9#9#9'Contacts.Contacts_Key <> 0'
      ''
      #9#9'Union All'
      ''
      #9'Select'
      #9#9#9'Orders.Menu_Name Group_Name,'
      #9#9#9'Orders.Course_Name,'
      #9#9#9'Orders.Item_Name,'
      #9#9#9'Orders.Size_Name,'
      #9#9#9'Orders.Qty as Item_Count,'
      
        '                                                Orders.Time_Stam' +
        'p,'
      #9#9#9'Security.From_Val as Old_Price,'
      
        '                                                Security.To_Val ' +
        'as New_Price,'
      
        '                                                Security.Note as' +
        ' Discount,'
      #9#9#9'Contacts.Name,'
      
        '                                               Security.Security' +
        '_Event'
      #9#9'From'
      
        #9#9#9'Security Left Join Orders on Security.Security_Ref = Orders.S' +
        'ecurity_Ref'
      
        #9#9#9'Inner Join Contacts on Orders.Loyalty_Key = Contacts.Contacts' +
        '_Key'
      #9#9'Where'
      #9#9#9'Orders.Time_Stamp >= '#39'01/01/05'#39' and'
      #9#9#9'Orders.Time_Stamp < '#39'12/10/05'#39' and'
      
        '                                                Security.Securit' +
        'y_Event  in ( '#39'Member Discount'#39','#39'Location Discount'#39' ) and'
      #9#9#9'Contacts.Contacts_Key <> 0'
      ''
      #9#9'Order By'
      #9#9#9'10,9,1,2,3,4;')
    Filtered = True
    OnFilterRecord = qrPatronCountFilterRecord
    Left = 328
    Top = 8
  end
  object ravLoyaltySales: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrLoyaltySales
    Left = 328
    Top = 160
  end
  object qrLoyaltySales: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'select'
      #9#9#9'Contacts.Member_Number,'
      #9#9#9'Contacts.Name,'
      #9#9#9'Archive.Time_Stamp,'
      #9#9#9'Archive.Menu_Name Group_Name,'
      #9#9#9'Archive.Course_Name,'
      #9#9#9'Cast(Archive.Item_Name as VarChar(50)) Item_Name,'
      #9#9#9'Archive.Size_Name,'
      
        '                                                Arcbill.Invoice_' +
        'Number,'
      
        '                                                sum(Archive.Poin' +
        'ts_Earned)as Earned,'
      #9#9#9'sum(Archive.Redeemed)as Redeemded,'
      #9#9#9'Sum(Archive.Qty) Item_Count,'
      #9#9#9'Sum(Archive.Price) Price,'
      #9#9#9'Sum(Archive.Price) PriceExcl,'
      #9#9#9'Sum(Archive.Cost) Cost,'
      #9#9#9'Sum(Archive.Cost) Profit'
      #9#9'From'
      #9#9#9'Security Left Join Archive on'
      #9#9#9#9'Security.Security_Ref = Archive.Security_Ref'
      #9#9#9'Left Join Contacts on'
      #9#9#9#9'Archive.Loyalty_Key = Contacts.Contacts_Key'
      #9'                               Left Join Arcbill on'
      #9#9#9#9'Archive.Arcbill_Key = Arcbill.Arcbill_Key'
      #9#9'Where'
      #9#9#9'Archive.Order_Type in (0,3) and'
      #9#9#9'Security.Time_Stamp >= '#39'01/01/04'#39' and'
      #9#9#9'Security.Time_Stamp <  '#39'01/01/05'#39'   and'
      #9#9#9'Security.Security_Event = '#39'Ordered By'#39' and'
      #9#9#9'Archive.Loyalty_Key <> 0'
      ''
      #9#9'Group By'
      #9#9#9'Contacts.Name,'
      #9#9#9'Contacts.Member_Number,'
      #9#9#9'Archive.Time_Stamp,'
      #9#9#9'Archive.Menu_Name,'
      #9#9#9'Archive.Course_Name,'
      #9#9#9'Archive.Item_Name,'
      #9#9#9'Archive.Size_Name,'
      
        '                                               Arcbill.Invoice_N' +
        'umber'
      '               '#9#9'Having'
      #9#9#9'Count(Archive.Archive_Key) > 0'
      ''
      #9#9'Union All'
      ''
      #9#9'Select'
      #9#9#9'Contacts.Member_Number,'
      #9#9#9'Contacts.Name,'
      #9#9#9'DayArchive.Time_Stamp,'
      #9#9#9'DayArchive.Menu_Name Group_Name,'
      #9#9#9'DayArchive.Course_Name,'
      #9#9#9'Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,'
      #9#9#9'DayArchive.Size_Name,'
      
        '                                                DayArcbill.Invoi' +
        'ce_Number,'
      #9#9#9'sum(DayArchive.Points_Earned) as Earned,'
      #9#9#9'sum(DayArchive.Redeemed) as Redeemed,'
      #9#9#9'Sum(DayArchive.Qty) Item_Count,'
      #9#9#9'Sum(DayArchive.Price) Price,'
      #9#9#9'Sum(DayArchive.Price) PriceExcl,'
      #9#9#9'Sum(DayArchive.Cost) Cost,'
      #9#9#9'Sum(DayArchive.Cost) Profit'
      #9#9'From'
      #9#9#9'Security Left Join DayArchive on'
      #9#9#9#9'Security.Security_Ref = DayArchive.Security_Ref'
      #9#9#9'Left Join Contacts on'
      #9#9#9#9'DayArchive.Loyalty_Key = Contacts.Contacts_Key'
      
        '                                               Left Join DayArcb' +
        'ill on'
      #9#9#9#9'DayArchive.Arcbill_Key = DayArcbill.Arcbill_Key'
      #9#9'Where'
      #9#9#9'DayArchive.Order_Type in (0,3) and'
      #9#9#9'Security.Time_Stamp >= '#39'01/01/04'#39' and'
      #9#9#9'Security.Time_Stamp <  '#39'01/01/05'#39' and'
      #9#9#9'Security.Security_Event = '#39'Ordered By'#39' and'
      #9#9#9'DayArchive.Loyalty_Key <> 0'
      ''
      #9#9'Group By'
      #9#9#9'Contacts.Name,'
      #9#9#9'Contacts.Member_Number,'
      #9#9#9'DayArchive.Time_Stamp,'
      #9#9#9'DayArchive.Menu_Name,'
      #9#9#9'DayArchive.Course_Name,'
      #9#9#9'DayArchive.Item_Name,'
      #9#9#9'DayArchive.Size_Name,'
      
        '                                                DayArcbill.Invoi' +
        'ce_Number'
      #9#9'Having'
      #9#9#9'Count(DayArchive.Archive_Key) > 0'
      ''
      #9#9'Order By'
      #9#9#9'1,2,3,4,5;')
    Filtered = True
    OnFilterRecord = qrPatronCountFilterRecord
    Left = 328
    Top = 112
  end
  object ravHappyHour: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrHappyHour
    Left = 328
    Top = 264
  end
  object qrHappyHour: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      #9'Select'
      #9#9#9'Archive.Order_Location Group_Name,'
      #9#9#9'Archive.Course_Name,'
      #9#9#9'Archive.Item_Name,'
      #9#9#9'Archive.Size_Name,'
      #9#9#9'Archive.GST_Percent,'
      #9#9#9'Sum(Archive.Qty) Item_Count,'
      
        #9#9#9'Cast(Sum(Archive.Price + Archive.Discount) as Numeric(15,4)) ' +
        'Price,'
      
        #9#9#9'Cast(Sum(Archive.Price_Level1) - Sum(Archive.Price + Archive.' +
        'Discount) as Numeric(15,4)) Difference,'
      #9#9#9'Cast(Sum(Archive.Cost) as Numeric(15,4)) Cost,'
      #9#9#9'Cast(Sum(Archive.Price_Level0) as Numeric(15,4)) PriceLevel0,'
      #9#9#9'Cast(Sum(Archive.Price_Level1) as Numeric(15,4)) PriceLevel1'
      #9#9'From'
      #9#9#9'Security Left Join Archive on'
      #9#9#9#9'Security.Security_Ref = Archive.Security_Ref'
      #9#9'Where'
      #9#9#9'Archive.Order_Type in (0,3) and'
      #9#9#9'Archive.Happy_Hour = '#39'T'#39' and'
      #9#9#9'Security.Time_Stamp >= '#39'09/01/04'#39' and'
      #9#9#9'Security.Time_Stamp < '#39'11/01/04'#39' and'
      #9#9#9'Security.Security_Event = '#39'Ordered By'#39' and'
      #9#9#9'Archive.Loyalty_KEY > 0'
      #9#9'Group By'
      #9#9#9'Archive.Order_Location,'
      #9#9#9'Archive.Course_Name,'
      #9#9#9'Archive.Item_Name,'
      #9#9#9'Archive.Size_Name,'
      #9#9#9'Archive.GST_Percent'
      #9#9'Having'
      #9#9#9'Count(Archive.Archive_Key) > 0'
      #9)
    Filtered = True
    Left = 326
    Top = 214
  end
  object ravLoyaltyPurchaseCount: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrLoyaltyPurchaseCount
    Left = 328
    Top = 384
  end
  object qrLoyaltyPurchaseCount: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9#9#9'cast('#39'Contact'#39' as Varchar(8)) Report_Type,'
      
        '                                               ContactFreebie.It' +
        'em_Name,'
      #9#9#9'ContactFreebie.Size_Name,'
      #9#9#9'ContactFreebie.Item_Count,'
      #9#9#9'Contacts.Name,'
      #9#9#9'sum(ItemSize.Loc_Sale_Count) Loc_Sale_Count,'
      #9#9#9'sum(ItemSize.Loc_Discount_Percent) Loc_Discount_Percent,'
      #9#9#9'sum(ItemSize.Mem_Sale_Count) Mem_Sale_Count,'
      #9#9#9'sum(ItemSize.Mem_Discount_Percent) Mem_Discount_Percent'
      #9#9'From'
      #9#9#9'ItemSize Left Join Item on'
      #9#9#9#9'ItemSize.Item_Key = Item.Item_Key'
      #9#9#9'Right join ContactFreebie on'
      
        #9#9#9#9'ItemSize.Size_Name = ContactFreebie.Size_Name and Item.Item_' +
        'Name = ContactFreebie.Item_Name'
      #9#9#9'Left Join Contacts on'
      #9#9#9#9'ContactFreebie.Contacts_Key = Contacts.Contacts_3RDParty_Key'
      #9#9'Group by'
      #9#9#9'ContactFreebie.Item_Name,'
      #9#9#9'ContactFreebie.Size_Name,'
      #9#9#9'ContactFreebie.Item_Count,'
      #9#9#9'Contacts.Name;'
      #9)
    Filtered = True
    Left = 326
    Top = 334
  end
  object qrBillTenders: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      #9#9#9'ArcBill.ArcBill_Key,'
      #9#9#9'Security.Time_Stamp,'
      #9#9#9'ArcBill.Invoice_Number,'
      #9#9#9'ArcBill.Total,'
      #9#9#9'ArcBill.Patron_Count,'
      #9#9#9'Security.Terminal_Name,'
      #9#9#9'Security.From_Val Staff_Name,'
      #9#9#9'ArcBillPay.Pay_Type,'
      #9#9#9'ArcBillPay.SubTotal,'
      #9#9#9'ArcBillPay.Voucher_Number,'
      #9#9#9'ArcBillPay.Note'
      #9#9'From'
      #9#9#9'ArcBill Left Join Security On'
      #9#9#9#9'ArcBill.Security_Ref = Security.Security_Ref'
      #9#9#9'Left Join ArcBillPay On'
      #9#9#9#9'ArcBill.ArcBill_Key = ArcBillPay.ArcBill_Key'
      #9#9'Where'
      #9#9#9'Security.Time_Stamp >= :StartTime And'
      #9#9#9'Security.Time_Stamp < :EndTime And'
      #9#9#9'Security.Security_Event = '#39'Billed By'#39';')
    Filtered = True
    Left = 326
    Top = 438
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
  object ravBillTenders: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrBillTenders
    Left = 320
    Top = 504
  end
  object sqlView: TIBSQL
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 32
    Top = 159
  end
  object cdsHalfHourlySummary: TClientDataSet
    Aggregates = <>
    FieldDefs = <>
    IndexDefs = <>
    Params = <>
    StoreDefs = True
    Left = 176
    Top = 408
  end
  object qrWeightedGP: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      
        #9#9#9'cast(sum(((Archive.Price + Archive.Discount) * 100 / (Archive' +
        '.GST_Percent + 100))  - (Archive.Cost)) /'
      
        #9#9#9'       sum((Archive.Price + Archive.Discount) * 100 / (Archiv' +
        'e.GST_Percent + 100) / 100)  as numeric(15, 4)) GP_Percent'
      #9#9'From'
      #9#9#9'Archive'
      #9#9'Where'
      #9#9#9'Archive.Time_Stamp_Billed >= '#39'06/01/05'#39' and'
      #9#9#9'Archive.Time_Stamp_Billed <  '#39'07/01/05'#39
      #9#9)
    Filtered = True
    OnFilterRecord = qrPatronCountFilterRecord
    Left = 592
    Top = 8
  end
  object ravWeightedGP: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrWeightedGP
    Left = 592
    Top = 56
  end
  object qrPatronTypes: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      #9#9'Select'
      #9#9#9'cast(Security.Time_Stamp as Date) Time_Stamp,'
      #9#9#9'PatronCount.Patron_Type,'
      #9#9#9'PatronCount.Patron_Count,'
      #9#9#9'ArcBill.Total'
      #9#9'From'
      #9#9#9'Security Left  Join ArcBill On'
      #9#9#9#9'Security.Security_Ref = ArcBill.Security_Ref'
      #9#9#9' right Join PatronCount On'
      #9#9#9#9'ArcBill.ArcBill_key = PatronCount.ArcBill_key'
      #9#9'Where'
      #9#9#9'Security.Time_Stamp >= '#39'11/01/05'#39' And'
      #9#9#9'Security.Time_Stamp < '#39'12/01/05'#39' And'
      #9#9#9'Security.Security_Event = '#39'Billed By'#39' And'
      #9#9#9'ArcBill.Patron_Count <> 0'
      #9#9' Union All'
      ''
      #9#9' Select'
      #9#9#9'cast(Security.Time_Stamp as Date) Time_Stamp,'
      #9#9#9'DayPatronCount.Patron_Type,'
      #9#9#9'DayPatronCount.Patron_Count,'
      #9#9#9'DayArcBill.Total'
      #9#9'From'
      #9#9#9'Security Left Join DayArcBill On'
      #9#9#9#9'Security.Security_Ref = DayArcBill.Security_Ref'
      #9#9#9' right Join DayPatronCount On'
      #9#9#9#9'DayArcBill.ArcBill_key = DayPatronCount.ArcBill_key'
      ''
      #9#9'Where'
      #9#9#9'Security.Time_Stamp >= '#39'11/01/05'#39' And'
      #9#9#9'Security.Time_Stamp < '#39'12/01/05'#39' And'
      #9#9#9'Security.Security_Event = '#39'Billed By'#39' And'
      #9#9#9'DayArcBill.Patron_Count <> 0'
      #9#9'Order By'
      #9#9#9'1,2;')
    Left = 592
    Top = 104
  end
  object ravPatronTypes: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrPatronTypes
    Left = 608
    Top = 256
  end
  object cdsMenu: TClientDataSet
    Aggregates = <>
    FieldDefs = <>
    IndexDefs = <>
    Params = <>
    StoreDefs = True
    Left = 168
    Top = 8
  end
  object qrMenuRecipe: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    AfterScroll = qrMenuRecipeAfterScroll
    SQL.Strings = (
      'Select'
      '   Menu.Menu_Name,'
      '   Course.Course_Name,'
      '   Item.Item_Name,'
      '   ItemSize.Size_Name,'
      'ItemSize.Price,'
      'ItemSize.Cost Menu_Cost,'
      '   ItemSize.Special_Price,'
      'ItemSize.Price - ItemSize.Special_Price Difference,'
      'ItemSize.Points_Percent,'
      'ItemSize.GST_Percent,'
      '   Recipe.Stock_Code Code,'
      '   Recipe.Qty,'
      '   Recipe.Stock_Location Location'
      'From'
      '   Menu Left Join Course On'
      '      Menu.Menu_Key = Course.Menu_Key'
      '   Left Join Item On'
      '      Course.Course_Key = Item.Course_Key'
      '   Left Join ItemSize On'
      '      Item.Item_Key = ItemSize.Item_Key'
      '   Left Join Recipe On'
      '      ItemSize.ItemSize_Key = Recipe.ItemSize_Key'
      'Where'
      '   Menu.Menu_Key = 145'
      'Order By'
      '   Menu.Menu_Name,'
      '   Course.Course_Name,'
      '   Item.Item_Name,'
      '   ItemSize.Size_Name')
    Left = 24
    Top = 208
  end
  object ravMenuRecipe: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrMenuRecipe
    Left = 64
    Top = 208
  end
  object qrLoyaltyAuditSummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'SELECT'
      #9'ARCBILL.BILLED_LOCATION AS LOCATION,'
      #9'CONTACTS.CONTACTS_KEY,'
      #9'CONTACTS.MEMBER_NUMBER,'
      #9'CONTACTS.NAME,'
      #9'CONTACTS.TOTAL_SPENT TOTAL_POINTS,'
      '                CONTACTS.INITIAL_EARNT_POINTS,'
      #9'POINTSTRANSACTIONS.ADJUSTMENT_TYPE,'
      #9'POINTSTRANSACTIONS.INVOICE_NUMBER,'
      #9'ARCBILL.TIME_STAMP,'
      #9'SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS,'
      #9'ARCBILL.TOTAL TOTAL_SPENT'
      'FROM'
      #9'CONTACTS INNER JOIN POINTSTRANSACTIONS ON'
      #9#9'  POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY'
      #9'INNER JOIN ARCBILL ON'
      #9#9'  POINTSTRANSACTIONS.INVOICE_NUMBER = ARCBILL.INVOICE_NUMBER'
      'WHERE'
      #9'CONTACTS.CONTACT_TYPE = 2'
      'GROUP BY'
      #9'ARCBILL.BILLED_LOCATION,'
      #9'CONTACTS.CONTACTS_KEY,'
      #9'CONTACTS.MEMBER_NUMBER,'
      #9'CONTACTS.NAME,'
      #9'CONTACTS.TOTAL_SPENT,'
      '                CONTACTS.INITIAL_EARNT_POINTS,'
      #9'POINTSTRANSACTIONS.ADJUSTMENT_TYPE,'
      #9'POINTSTRANSACTIONS.INVOICE_NUMBER,'
      #9'ARCBILL.TIME_STAMP,'
      #9'ARCBILL.TOTAL'
      ''
      'UNION ALL'
      ''
      'SELECT'
      #9'DAYARCBILL.BILLED_LOCATION AS LOCATION,'
      #9'CONTACTS.CONTACTS_KEY,'
      #9'CONTACTS.MEMBER_NUMBER,'
      #9'CONTACTS.NAME,'
      #9'CONTACTS.TOTAL_SPENT TOTAL_POINTS,'
      '                CONTACTS.INITIAL_EARNT_POINTS,'
      #9'POINTSTRANSACTIONS.ADJUSTMENT_TYPE,'
      #9'POINTSTRANSACTIONS.INVOICE_NUMBER,'
      #9'DAYARCBILL.TIME_STAMP,'
      #9'SUM(POINTSTRANSACTIONS.ADJUSTMENT) POINTS,'
      #9'DAYARCBILL.TOTAL TOTAL_SPENT'
      'FROM'
      #9'CONTACTS INNER JOIN POINTSTRANSACTIONS ON'
      #9#9'  POINTSTRANSACTIONS.CONTACTS_KEY = CONTACTS.CONTACTS_KEY'
      #9'INNER JOIN DAYARCBILL ON'
      
        #9#9'  POINTSTRANSACTIONS.INVOICE_NUMBER = DAYARCBILL.INVOICE_NUMBE' +
        'R'
      'WHERE'
      #9'CONTACTS.CONTACT_TYPE = 2'
      'GROUP BY'
      #9'DAYARCBILL.BILLED_LOCATION,'
      #9'CONTACTS.CONTACTS_KEY,'
      #9'CONTACTS.MEMBER_NUMBER,'
      #9'CONTACTS.NAME,'
      #9'CONTACTS.TOTAL_SPENT,'
      '                CONTACTS.INITIAL_EARNT_POINTS,'
      #9'POINTSTRANSACTIONS.ADJUSTMENT_TYPE,'
      #9'POINTSTRANSACTIONS.INVOICE_NUMBER,'
      #9'DAYARCBILL.TIME_STAMP,'
      #9'DAYARCBILL.TOTAL'
      ''
      'ORDER BY'
      #9'4,'
      #9'6')
    Left = 408
    Top = 736
  end
  object ravLoyaltyAuditSummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrLoyaltyAuditSummary
    Left = 520
    Top = 720
  end
  object qrMenu3rdParty: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select'
      '   Menu.Menu_Name,'
      '   Course.Course_Name,'
      '   Item.Item_Name,'
      '   ItemSize.ItemSize_Key,'
      '   ItemSize.Size_Name,'
      '   ItemSize.Price,'
      '   ItemSize.Special_Price,'
      '   ItemSize.Barcode,'
      '   ThirdPartyCodes.Code'
      'From'
      '   Menu Left Join Course On'
      '      Menu.Menu_Key = Course.Menu_Key'
      '   Left Join Item On'
      '      Course.Course_Key = Item.Course_Key'
      '   Left Join ItemSize On'
      '      Item.Item_Key = ItemSize.Item_Key'
      '   Left Join ThirdPartyCodes On'
      
        '       ItemSize.ThirdPartyCodes_Key = ThirdPartyCodes.ThirdParty' +
        'Codes_Key'
      'Order By'
      '   Menu.Menu_Name,'
      '   Course.Course_Name,'
      '   Item.Item_Name,'
      '   ItemSize.Size_Name')
    Left = 112
    Top = 752
  end
  object ravMenu3rdParty: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrMenu3rdParty
    Left = 224
    Top = 752
  end
  object qrCashupRecon: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      #9#9'Select'
      #9#9#9'Zeds.Z_Key,'
      #9#9#9'Zeds.Time_Stamp,'
      #9#9#9'Zeds.Terminal_Name,'
      #9#9#9'Zeds.Adjustments as Total_Variance,'
      #9#9#9'Security.From_Val as Cashier,'
      #9#9#9'BlindBalance.Payment as Payment_Type,'
      #9#9#9'BlindBalance.Payment_Group,'
      #9#9#9'BlindBalance.Payment_Trans_Qty as Transaction_Qty,'
      #9#9#9'BlindBalance.System_Balance,'
      #9#9#9'BlindBalance.Office_Balance,'
      
        #9#9#9'BlindBalance.System_Balance - BlindBalance.Office_Balance as ' +
        'Variance'
      #9#9'From'
      #9#9#9'Zeds Inner Join Security on'
      #9#9#9#9'Zeds.Security_Ref = Security.Security_Ref'
      #9#9#9'Left Join BlindBalance on'
      #9#9#9#9'Zeds.Z_Key = BlindBalance.Z_Key'
      #9#9'Where'
      
        #9#9#9'Zeds.Time_Stamp > '#39'1/12/2008'#39' and Zeds.Time_Stamp < '#39'1/19/200' +
        '8'#39
      #9#9'Order By'
      #9#9#9'Zeds.Time_Stamp,'
      #9#9#9'BlindBalance.Z_Key,'
      #9#9#9'BlindBalance.Payment_Group,'
      #9#9#9'BlindBalance.Payment')
    Left = 112
    Top = 816
  end
  object ravCashupRecon: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCashupRecon
    Left = 224
    Top = 816
  end
  object ravMembershipAuditPointsBreakdown: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrMembershipAuditPointsBreakdown
    Left = 856
    Top = 96
  end
  object qrMembershipAuditPointsBreakdown: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    CachedUpdates = True
    Left = 856
    Top = 144
  end
  object ravExtendedMembershipAuditPointsBreakdown: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrExtendedMembershipAuditPointsBreakdown
    Left = 856
    Top = 200
  end
  object qrExtendedMembershipAuditPointsBreakdown: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    CachedUpdates = True
    Left = 856
    Top = 248
  end
  object qrInvoice: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      'select '
      
        '           groups.name, contacts.name contactname, invoices.TOTA' +
        'L_INC,            INVOICES.INVOICE_NUMBER, DAYARCBILL.TIME_STAMP' +
        ',            DAYARCBILL.STAFF_NAME '
      'from '
      '           CONTACTGROUPS'
      'left join groups on           '
      ' CONTACTGROUPS.GROUPS_KEY = groups.GROUPS_KEY'
      'left join contacts on '
      '           contactgroups.CONTACTS_KEY = contacts.CONTACTS_KEY '
      'left join invoices on '
      '           invoices.CONTACTS_KEY = contacts.CONTACTS_KEY'
      'left join DAYARCBILL on '
      '           DAYARCBILL.INVOICE_KEY = invoices.INVOICE_KEY'
      'where '
      '           dayarcbill.time_stamp >= :StartTime and '
      '           dayarcbill.time_stamp < :EndTime and'
      '           contacts.account_profile = '#39'1'#39
      ''
      'Union all'
      ''
      'select '
      
        '           groups.name, contacts.name contactname, invoices.TOTA' +
        'L_INC,                       INVOICES.INVOICE_NUMBER, ARCBILL.TI' +
        'ME_STAMP,                       ARCBILL.STAFF_NAME '
      'from '
      '           CONTACTGROUPS'
      'left join groups on            '
      '           CONTACTGROUPS.GROUPS_KEY = groups.GROUPS_KEY'
      'left join contacts on '
      '           contactgroups.CONTACTS_KEY = contacts.CONTACTS_KEY '
      'left join invoices on '
      '           invoices.CONTACTS_KEY = contacts.CONTACTS_KEY'
      'left join ARCBILL on '
      '           ARCBILL.INVOICE_KEY = invoices.INVOICE_KEY'
      'where '
      '           arcbill.time_stamp >= :StartTime and '
      '           arcbill.time_stamp < :EndTime and'
      '           contacts.account_profile = '#39'1'#39';'
      ''
      ''
      ''
      ''
      ''
      ''
      ''
      ''
      ''
      ''
      ''
      ''
      ''
      ''
      '')
    Filtered = True
    Left = 856
    Top = 476
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
      end
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
  object ravInvoice: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrInvoice
    Left = 768
    Top = 538
  end
  object qrSkimming: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      'Select '
      'Refloat_Skim.Amount,'
      'Refloat_Skim.Staff,           '
      ' Refloat_Skim.Terminal_Name, '
      '            Refloat_Skim.Time_Stamp, '
      '            Refloat_Skim.Transaction_type,'
      '            Refloat_Skim.Reasons,'
      '            zeds.initial_float '
      '        From '
      
        '            Refloat_Skim left join zeds on refloat_skim.Z_KEY = ' +
        'zeds.z_key'
      '        Where '
      '            Refloat_Skim.Time_Stamp >= :StartTime And '
      '            Refloat_Skim.Time_Stamp < :EndTime'
      '        Order by 4;')
    Left = 856
    Top = 427
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
  object ravSkimming: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSkimming
    Left = 904
    Top = 473
  end
  object qrRefloat: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      'Select '
      '            Amount, '
      '            Staff, '
      '            Terminal_Name, '
      '            Time_Stamp, '
      '            Transaction_type,'
      '            Reasons '
      '        From '
      '            Refloat_Skim '
      '        Where '
      '            Time_Stamp >= :StartTime And '
      '            Time_Stamp < :EndTime And '
      '            (Transaction_Type = '#39'Refloat'#39');')
    Left = 856
    Top = 392
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
  object ravRefloat: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrRefloat
    Left = 904
    Top = 392
  end
  object qrInvoiceDetailed: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '        select '
      '            arcbill.Staff_Name, '
      '            arcbill.time_stamp, '
      '            INVOICES.TOTAL_INC, '
      '            ARCHIVE.ITEM_NAME, '
      '            ARCHIVE.PRICE, '
      '            ARCHIVE.QTY, '
      '            INVOICES.INVOICE_NUMBER, '
      '            INVOICES.CONTACTS_KEY, '
      '            CONTACTS.name, '
      '            contacts.member_number, '
      '            INVOICES.CLOSED '
      '        from ARCBILL '
      
        '            left join ARCHIVE on ARCBILL.ARCBILL_KEY = ARCHIVE.A' +
        'RCBILL_KEY '
      
        '            left join INVOICES on ARCBILL.INVOICE_KEY = INVOICES' +
        '.INVOICE_KEY '
      
        '            left join CONTACTS on INVOICES.CONTACTS_KEY = CONTAC' +
        'TS.CONTACTS_KEY '
      '            where arcbill.time_stamp >= :StartTime and '
      '            arcbill.time_stamp < :EndTime and'
      '            arcbill.invoice_key <> '#39'0'#39
      ''
      '            union all '
      ''
      '        select '
      '            dayarcbill.Staff_Name, '
      '            dayarcbill.time_stamp, '
      '            INVOICES.TOTAL_INC, '
      '            dayARCHIVE.ITEM_NAME, '
      '            dayARCHIVE.PRICE, '
      '            dayARCHIVE.QTY, '
      '            INVOICES.INVOICE_NUMBER, '
      '            INVOICES.CONTACTS_KEY, '
      '            CONTACTS.name, '
      '            contacts.member_number, '
      '            INVOICES.CLOSED '
      '        from dayARCBILL '
      
        '            left join dayARCHIVE on dayARCBILL.ARCBILL_KEY = day' +
        'ARCHIVE.ARCBILL_KEY '
      
        '            left join INVOICES on dayARCBILL.INVOICE_KEY = INVOI' +
        'CES.INVOICE_KEY '
      
        '            left join CONTACTS on INVOICES.CONTACTS_KEY = CONTAC' +
        'TS.CONTACTS_KEY '
      '            where dayarcbill.time_stamp >= :StartTime and '
      '            dayarcbill.time_stamp < :EndTime and'
      '            dayarcbill.invoice_key <> '#39'0'#39
      ''
      '            union all '
      ''
      '        select '
      '            orders.Server_Name Staff_Name, '
      '            orders.TIME_STAMP, '
      '            INVOICES.TOTAL_INC, '
      '            orders.ITEM_NAME, '
      '            orders.PRICE, '
      '            orders.QTY, '
      '            INVOICES.INVOICE_NUMBER, '
      '            INVOICES.CONTACTS_KEY, '
      '            CONTACTS.NAME, '
      '            contacts.member_number, '
      '            INVOICES.CLOSED '
      '        from INVOICES '
      
        '            left join TAB on INVOICES.INVOICE_KEY = TAB.INVOICE_' +
        'KEY '
      '            left join orders on TAB.TAB_KEY = orders.TAB_KEY '
      
        '            left join CONTACTS on INVOICES.CONTACTS_KEY = CONTAC' +
        'TS.CONTACTS_KEY '
      '           where orders.time_stamp >= :StartTime and '
      '           orders.time_stamp < :EndTime and'
      '           INVOICES.CLOSED = '#39'F'#39
      ''
      'order by 8, 6;'
      '')
    Left = 856
    Top = 512
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
      end
      item
        DataType = ftUnknown
        Name = 'StartTime'
        ParamType = ptUnknown
      end
      item
        DataType = ftUnknown
        Name = 'EndTime'
        ParamType = ptUnknown
      end
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
  object ravInvoiceDetailed: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrInvoiceDetailed
    Left = 936
    Top = 456
  end
  object qrLostRevenue: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      '        select Writeoff.item, '
      '            WRITEOFF.TIME_STAMP, '
      '            WRITEOFF.REASONS, '
      '            WRITEOFF.AMOUNT, '
      '            WRITEOFF.SIZE_NAME, '
      '            ARCCATEGORIES.CATEGORY, '
      '            CATEGORYGROUPS.NAME, '
      '            MENU.MENU_NAME, '
      '            COURSE.COURSE_NAME '
      '        from WRITEOFF '
      
        '            left join ARCCATEGORIES on writeoff.CATEGORY_KEY = A' +
        'RCCATEGORIES.CATEGORY_KEY '
      
        '            left join CATEGORYGROUPS on WRITEOFF.GROUP_KEY = CAT' +
        'EGORYGROUPS.CATEGORYGROUPS_KEY '
      '            left join MENU on WRITEOFF.MENU_KEY = MENU.MENU_KEY '
      
        '            left join COURSE on WRITEOFF.COURSE_KEY = COURSE.COU' +
        'RSE_KEY '
      '        where '
      #9'WRITEOFF.TIME_STAMP >= :StartTime and '
      #9'WRITEOFF.TIME_STAMP < :EndTime'
      '        order by 9, 8;'
      ''
      '')
    Left = 680
    Top = 376
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
  object ravLostRevenue: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrLostRevenue
    Left = 776
    Top = 376
  end
  object qrSalesJournal1: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      
        '        select arcbill_key, datetime, pos, transaction_number, s' +
        'um(price * qty) as price, sum(discount1) as discount1, sum(disco' +
        'unt2) as discount2, sum(discount3) as discount3, sum(discount4) ' +
        'as discount4, sum(price + discount1) as semitotal, sum(price + d' +
        'iscount4) as total  from '
      '        (select '
      '            a.arcbill_key, '
      '            b.time_stamp datetime, '
      '            b.terminal_name POS, '
      '            b.invoice_number transaction_number, '
      
        '            sum(case when lower(c.name) not like '#39'%service%'#39' and' +
        ' lower(c.name) not like '#39'%vat%'#39' and lower(c.name) not like '#39'%gst' +
        '%'#39' then c.discounted_value else 0 end) as discount1, '
      
        '            sum(case when lower(c.name) like '#39'%service%'#39' then c.' +
        'discounted_value else 0 end) as discount2, '
      
        '            sum(case when lower(c.name) like '#39'%vat%'#39' or lower(c.' +
        'name) like '#39'%gst%'#39' then c.discounted_value else 0 end) as discou' +
        'nt3, '
      '            a.price price, '
      '            a.qty qty, '
      '            sum(c.discounted_value) as discount4 '
      '        from dayarchive a '
      
        '            left join dayarcbill b on a.arcbill_key = b.arcbill_' +
        'key '
      
        '            left join dayarcorderdiscounts c on a.archive_key = ' +
        'c.archive_key'
      
        '        group by a.arcbill_key, b.time_stamp, b.terminal_name, b' +
        '.invoice_number, b.total, a.price, a.qty '
      ''
      '        union all '
      ''
      '        select '
      '            a.arcbill_key, '
      '            b.time_stamp datetime, '
      '            b.terminal_name POS, '
      '            b.invoice_number transaction_number, '
      
        '            sum(case when lower(c.name) not like '#39'%service%'#39' and' +
        ' lower(c.name) not like '#39'%vat%'#39' and lower(c.name) not like '#39'%gst' +
        '%'#39' then c.discounted_value else 0 end) as discount1, '
      
        '            sum(case when lower(c.name) like '#39'%service%'#39' then c.' +
        'discounted_value else 0 end) as discount2, '
      
        '            sum(case when lower(c.name) like '#39'%vat%'#39' or lower(c.' +
        'name) like '#39'%gst%'#39' then c.discounted_value else 0 end) as discou' +
        'nt3, '
      '            a.price price, '
      '            a.qty qty, '
      '            sum(c.discounted_value) as discount4 '
      '        from archive a '
      
        '            left join arcbill b on a.arcbill_key = b.arcbill_key' +
        ' '
      
        '            left join arcorderdiscounts c on a.archive_key = c.a' +
        'rchive_key'
      
        '        group by a.arcbill_key, b.time_stamp, b.terminal_name, b' +
        '.invoice_number, b.total, a.price, a.qty) '
      '        group by transaction_number, arcbill_key, pos, datetime'
      '        order by arcbill_key;')
    Left = 408
    Top = 785
  end
  object ravSalesJournal1: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesJournal1
    Left = 520
    Top = 784
  end
  object qrSalesJournal2: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      '        Select '
      '            DayArcBill.Total, '
      '            DayArcBill.Invoice_Number, '
      '            DayArcBill.Staff_Name, '
      '            DayArcBill.Terminal_Name, '
      '            DayArcBill.Time_Stamp, '
      '            DayArchive.Price, '
      '            DayArchive.Discount, '
      '            DayArchive.Discount_reason '
      '        from '
      '            DayArcBill left join DayArchive on '
      '            DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key '
      '        where lower(Discount_Reason) like '#39'service'#39' and'
      #9#9#9'DayArcBill.Time_Stamp >= :StartTime and '
      #9#9#9'DayArcBill.Time_Stamp < :EndTime '
      '        '
      '            Union all'
      '            '
      '        Select '
      '            ArcBill.Total, '
      '            ArcBill.Invoice_Number, '
      '            ArcBill.Staff_Name, '
      '            ArcBill.Terminal_Name, '
      '            ArcBill.Time_Stamp, '
      '            Archive.Price, '
      '            Archive.Discount, '
      '            Archive.Discount_reason '
      '        from '
      '            ArcBill left join Archive on '
      '            ArcBill.ArcBill_Key = Archive.ArcBill_Key '
      '        where lower(Discount_Reason) like '#39'service'#39' and'
      #9#9#9'ArcBill.Time_Stamp >= :StartTime and '
      #9#9#9'ArcBill.Time_Stamp < :EndTime ')
    Left = 912
    Top = 15
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
      end
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
  object ravSalesJournal2: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesJournal2
    Left = 968
    Top = 7
  end
  object qrSalesJournal3: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      '        Select '
      '            DayArcBill.Total, '
      '            DayArcBill.Invoice_Number, '
      '            DayArcBill.Staff_Name, '
      '            DayArcBill.Terminal_Name, '
      '            DayArcBill.Time_Stamp, '
      '            DayArchive.Price, '
      '            DayArchive.Discount, '
      '            DayArchive.Discount_reason '
      '        from '
      '            DayArcBill left join DayArchive on '
      '            DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key '
      
        '        where lower(Discount_Reason) like '#39'vat'#39' or lower(Discoun' +
        't_Reason) like '#39'gst'#39' and'
      #9#9#9'DayArcBill.Time_Stamp >= :StartTime and '
      #9#9#9'DayArcBill.Time_Stamp < :EndTime'
      '        '
      '            Union all'
      '            '
      '        Select '
      '            ArcBill.Total, '
      '            ArcBill.Invoice_Number, '
      '            ArcBill.Staff_Name, '
      '            ArcBill.Terminal_Name, '
      '            ArcBill.Time_Stamp, '
      '            Archive.Price, '
      '            Archive.Discount, '
      '            Archive.Discount_reason '
      '        from '
      '            ArcBill left join Archive on '
      '            ArcBill.ArcBill_Key = Archive.ArcBill_Key '
      
        '        where lower(Discount_Reason) like '#39'vat'#39' or lower(Discoun' +
        't_Reason) like '#39'gst'#39' and'
      #9#9#9'ArcBill.Time_Stamp >= :StartTime and '
      #9#9#9'ArcBill.Time_Stamp < :EndTime ')
    Left = 164
    Top = 328
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
      end
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
  object ravSalesJournal3: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesJournal3
    Left = 848
    Top = 16
  end
  object qrSalesJournal4: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      '        Select '
      '            DayArcBill.Total, '
      '            DayArcBill.Invoice_Number, '
      '            DayArcBill.Staff_Name, '
      '            DayArcBill.Terminal_Name, '
      '            DayArcBill.Time_Stamp, '
      '            DayArchive.Price, '
      '            DayArchive.Discount, '
      '            DayArchive.Discount_reason '
      '        from '
      '            DayArcBill left join DayArchive on '
      '            DayArcBill.ArcBill_Key = DayArchive.ArcBill_Key '
      '        where '
      #9#9#9'DayArcBill.Time_Stamp >= :StartTime and '
      #9#9#9'DayArcBill.Time_Stamp < :EndTime'
      '        '
      '            Union all'
      '            '
      '        Select '
      '            ArcBill.Total, '
      '            ArcBill.Invoice_Number, '
      '            ArcBill.Staff_Name, '
      '            ArcBill.Terminal_Name, '
      '            ArcBill.Time_Stamp, '
      '            Archive.Price, '
      '            Archive.Discount, '
      '            Archive.Discount_reason '
      '        from '
      '            ArcBill left join Archive on '
      '            ArcBill.ArcBill_Key = Archive.ArcBill_Key '
      '        where '
      #9#9#9'ArcBill.Time_Stamp >= :StartTime and '
      #9#9#9'ArcBill.Time_Stamp < :EndTime ')
    Left = 408
    Top = 840
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
      end
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
  object ravSalesJournal4: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesJournal4
    Left = 520
    Top = 840
  end
  object qrDayConsumption: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      #9#9'Select '
      '            Archive.Time_Stamp,'
      
        '            Extract (Minute From Archive.Time_Stamp) Order_Minut' +
        'e,'
      #9#9#9'Extract (Hour From Archive.Time_Stamp) Order_Hour,'
      #9#9#9'Extract (Day From Archive.Time_Stamp) Order_Day,'
      #9#9#9'Extract (Month From Archive.Time_Stamp) Order_Month,'
      #9#9#9'Extract (Year From Archive.Time_Stamp) Order_Year,'#9#9
      
        #9#9#9'Cast(Sum(Archive.Qty * Archive.Price + Archive.Discount) as N' +
        'umeric(15,4)) Price,'
      
        #9#9#9'Cast(Sum((Archive.Qty * Archive.Price + Archive.Discount) * 1' +
        '00 / (Archive.GST_Percent + 100)) as Numeric(15,4)) PriceExc,'
      #9#9#9'Cast(Sum(Archive.Cost * Archive.Qty) as Numeric(15,4)) Cost '
      #9#9'From '
      #9#9#9'Security Left Join Archive on '
      #9#9#9'Security.Security_Ref = Archive.Security_Ref '
      '         Left Join ArcCategories on '
      '         '#9'Archive.Category_Key = ArcCategories.Category_Key '
      #9#9#9'Left Join CategoryGroups on '
      
        #9#9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGro' +
        'ups_Key '
      #9#9'Where '
      #9#9#9'(Archive.Order_Type = 3 or '
      #9#9#9'Archive.Order_Type = 0) and '
      #9#9#9'Security.Time_Stamp >= :StartTime and '
      #9#9#9'Security.Time_Stamp < :EndTime and '
      #9#9#9'Security.Security_Event = '#39'Ordered By'#39' '
      #9#9'Group By '
      '            Archive.Time_Stamp'
      #9#9'Having '
      #9#9#9'Count(Archive.Archive_Key) > 0 '
      ''
      #9#9'Union All '
      '    '
      #9#9'Select '
      '            DayArchive.Time_Stamp,'
      
        '            Extract (Minute From DayArchive.Time_Stamp) Order_Mi' +
        'nute,'
      #9#9#9'Extract (Hour From DayArchive.Time_Stamp) Order_Hour,'
      #9#9#9'Extract (Day From DayArchive.Time_Stamp) Order_Day,'
      #9#9#9'Extract (Month From DayArchive.Time_Stamp) Order_Month,'
      
        #9#9#9'Extract (Year From DayArchive.Time_Stamp) Order_Year,'#9#9#9'     ' +
        '     '
      
        #9#9#9'Cast(Sum(DayArchive.Qty * DayArchive.Price + DayArchive.Disco' +
        'unt) as Numeric(15,4)) Price,'
      
        #9#9#9'Cast(Sum((DayArchive.Qty * DayArchive.Price + DayArchive.Disc' +
        'ount) * 100 / (DayArchive.GST_Percent + 100)) as Numeric(15,4)) ' +
        'PriceExc,'
      
        #9#9#9'Cast(Sum(DayArchive.Cost * DayArchive.Qty) as Numeric(15,4)) ' +
        'Cost '
      #9#9'From'
      #9#9#9'Security Left Join DayArchive on '
      '            Security.Security_Ref = DayArchive.Security_Ref '
      '         Left Join ArcCategories on '
      '         '#9'DayArchive.Category_Key = ArcCategories.Category_Key '
      #9#9#9'Left Join CategoryGroups on '
      
        #9#9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGro' +
        'ups_Key '
      #9#9'Where '
      #9#9#9'(DayArchive.Order_Type = 3 or '
      #9#9#9'DayArchive.Order_Type = 0) and '
      #9#9#9'Security.Time_Stamp >= :StartTime and '
      #9#9#9'Security.Time_Stamp < :EndTime and '
      #9#9#9'Security.Security_Event = '#39'Ordered By'#39' '
      #9#9'Group By '
      '            DayArchive.Time_Stamp'
      #9#9'Having '
      #9#9#9'Count(DayArchive.Archive_Key) > 0 '
      #9#9'Order By '
      #9#9#9'1')
    Left = 672
    Top = 712
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
      end
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
  object ravDayConsumption: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDayConsumption
    Left = 768
    Top = 712
  end
  object ChefMateTrans: TIBTransaction
    DefaultDatabase = dmChefMateData.dbChefMate
    Left = 32
    Top = 104
  end
  object ravConsumptionExcSurcharge: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrConsumptionExcSurcharge
    Left = 1067
    Top = 520
  end
  object qrConsumptionExcSurcharge: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      #9#9'Select '
      #9#9#9'CategoryGroups.Name Group_Name,'
      #9#9#9'ArcCategories.Category Course_Name,'
      #9#9#9'Cast(Archive.Item_Name as VarChar(50)) Item_Name,'
      #9#9#9'Archive.Size_Name,'
      #9#9#9'Sum(Archive.Qty) Item_Count,'
      #9#9#9'Cast(Sum( '
      '                case when Archive.Discount > 0 '
      '                then (Archive.Qty * Archive.Price) '
      
        '                else (Archive.Qty * Archive.Price + Archive.Disc' +
        'ount) '
      '                end) as Numeric(15,4)) Price, '
      #9#9#9'Cast(Sum( '
      '                case when Archive.Discount > 0 '
      
        '                then (Archive.Qty * Archive.Price) * 100 / (Arch' +
        'ive.GST_Percent + 100) '
      
        '                else (Archive.Qty * Archive.Price + Archive.Disc' +
        'ount) * 100 / (Archive.GST_Percent + 100) '
      '                end) as Numeric(15,4)) PriceExc, '
      #9#9#9'Cast(Sum(Archive.Cost * Archive.Qty) as Numeric(15,4)) Cost '
      #9#9'From '
      #9#9#9'Security Left Join Archive on '
      #9#9#9#9'Security.Security_Ref = Archive.Security_Ref '
      '         Left Join ArcCategories on '
      '         '#9'Archive.Category_Key = ArcCategories.Category_Key '
      #9#9#9'Left Join CategoryGroups on '
      
        #9#9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGro' +
        'ups_Key '
      #9#9'Where '
      #9#9#9'(Archive.Order_Type = 3 or '
      #9#9#9'Archive.Order_Type = 0) and '
      #9#9#9'Security.Time_Stamp >= '#39'29.03.2014, 09:55:06.000'#39' and '
      #9#9#9'Security.Time_Stamp < '#39'29.04.2014, 09:55:06.000'#39' and '
      #9#9#9'Security.Security_Event = '#39'Ordered By'#39' '
      #9#9'Group By '
      #9#9#9'CategoryGroups.Name,'
      #9#9#9'ArcCategories.Category,'
      #9#9#9'Archive.Item_Name,'
      #9#9#9'Archive.Size_Name '
      #9#9'Having '
      #9#9#9'Count(Archive.Archive_Key) > 0 '
      ''
      #9#9'Union All '
      ''
      #9#9'Select '
      #9#9#9'CategoryGroups.Name Group_Name,'
      #9#9#9'ArcCategories.Category Course_Name,'
      #9#9#9'Cast(DayArchive.Item_Name as VarChar(50)) Item_Name,'
      #9#9#9'DayArchive.Size_Name,'
      #9#9#9'Sum(DayArchive.Qty) Item_Count,'
      #9#9#9'Cast(Sum( '
      '                case when DayArchive.Discount > 0 '
      '                then (DayArchive.Qty * DayArchive.Price) '
      
        '                else (DayArchive.Qty * DayArchive.Price + DayArc' +
        'hive.Discount) '
      '                end) as Numeric(15,4)) Price, '
      #9#9#9'Cast(Sum( '
      '                case when DayArchive.Discount > 0 '
      
        '                then (DayArchive.Qty * DayArchive.Price) * 100 /' +
        ' (DayArchive.GST_Percent + 100) '
      
        '                else (DayArchive.Qty * DayArchive.Price + DayArc' +
        'hive.Discount) * 100 / (DayArchive.GST_Percent + 100) '
      '                end) as Numeric(15,4)) PriceExc, '
      
        #9#9#9'Cast(Sum(DayArchive.Cost * DayArchive.Qty) as Numeric(15,4)) ' +
        'Cost '
      #9#9'From '
      #9#9#9'Security Left Join DayArchive on '
      #9#9#9'Security.Security_Ref = DayArchive.Security_Ref '
      '         Left Join ArcCategories on '
      '         '#9'DayArchive.Category_Key = ArcCategories.Category_Key '
      #9#9#9'Left Join CategoryGroups on '
      
        #9#9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGro' +
        'ups_Key '
      #9#9'Where '
      #9#9#9'(DayArchive.Order_Type = 3 or '
      #9#9#9'DayArchive.Order_Type = 0) and '
      #9#9#9'Security.Time_Stamp >= '#39'29.03.2014, 09:55:06.000'#39'  and '
      #9#9#9'Security.Time_Stamp < '#39'29.04.2014, 09:55:06.000'#39'  and '
      #9#9#9'Security.Security_Event = '#39'Ordered By'#39' '
      #9#9'Group By '
      #9#9#9'CategoryGroups.Name,'
      #9#9#9'ArcCategories.Category,'
      #9#9#9'DayArchive.Item_Name,'
      #9#9#9'DayArchive.Size_Name '
      #9#9'Having '
      #9#9#9'Count(DayArchive.Archive_Key) > 0 '
      ''
      #9#9'Union All '
      ''
      #9#9'Select '
      #9#9#9'CategoryGroups.Name Group_Name,'
      #9#9#9'ArcCategories.Category Course_Name,'
      #9#9#9'Orders.Item_Name,'
      #9#9#9'Orders.Size_Name,'
      #9#9#9'Sum(Orders.Qty) Item_Count,'
      #9#9#9'Cast(Sum( '
      '                case when Orders.Discount > 0 '
      '                then (Orders.Qty * Orders.Price) '
      
        '                else (Orders.Qty * Orders.Price + Orders.Discoun' +
        't) '
      '                end) as Numeric(15,4)) Price, '
      #9#9#9'Cast(Sum('
      '                case when Orders.Discount > 0 '
      
        '                then (Orders.Qty * Orders.Price) * 100 / (Orders' +
        '.GST_Percent + 100) '
      
        '                else (Orders.Qty * Orders.Price + Orders.Discoun' +
        't) * 100 / (Orders.GST_Percent + 100) '
      '                end) as Numeric(15,4)) PriceExc, '
      #9#9#9'Cast(Sum(Orders.Cost * Orders.Qty) as Numeric(15,4)) Cost '
      #9#9'From '
      #9#9#9'Security Left Join Orders on '
      #9#9#9'Security.Security_Ref = Orders.Security_Ref '
      '         Left Join ArcCategories on '
      '         '#9'Orders.Category_Key = ArcCategories.Category_Key '
      #9#9#9'Left Join CategoryGroups on '
      
        #9#9#9'ArcCategories.CategoryGroups_Key = CategoryGroups.CategoryGro' +
        'ups_Key '
      #9#9'Where '
      #9#9#9'Security.Time_Stamp >= '#39'29.03.2014, 09:55:06.000'#39'  and '
      #9#9#9'Security.Time_Stamp < '#39'29.04.2014, 09:55:06.000'#39'  and '
      #9#9#9'Security.Security_Event = '#39'Ordered By'#39' '
      #9#9'Group By '
      #9#9#9'CategoryGroups.Name,'
      #9#9#9'ArcCategories.Category,'
      #9#9#9'Orders.Item_Name,'
      #9#9#9'Orders.Size_Name '
      #9#9'Having '
      #9#9#9'Count(Orders.Order_Key) > 0 '
      ''
      #9'Order By '
      #9#9#9'1,2,3,4')
    Left = 971
    Top = 520
  end
  object qrDiscountsSurchargesTotals: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      'Select '
      '     Distinct Security.From_Val as DISCOUNT_NAME, '
      '     abs(cast(sum(ARCHIVE.DISCOUNT) as numeric(15,4))) as TOTAL'
      'From '
      '     Security Left Join ArcBill On '
      '     Security.Security_Ref = ArcBill.Security_Ref '
      
        '     Left Join Archive On ArcBill.ArcBill_Key = Archive.ArcBill_' +
        'Key '
      'Where '
      '     Security.Time_Stamp >= '#39'29.04.2014'#39' and '
      '     Security.Time_Stamp < '#39'30.04.2014'#39' and '
      '     Security.Security_Event = '#39'Discounted By'#39' and'
      '     ArcBill.Invoice_Number Is Not Null and '
      '     Archive.Archive_Key Is Not Null'
      'Group by'
      '     Security.From_Val'
      'Order By '
      '     Security.From_Val')
    Left = 1019
    Top = 576
  end
  object ravDiscountsSurchargesTotals: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDiscountsSurchargesTotals
    Left = 1067
    Top = 568
  end
  object ravDSR: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSR
    Left = 16
    Top = 336
  end
  object qrDSR: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 16
    Top = 400
  end
  object qrDSRTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 16
    Top = 456
  end
  object qrDSRInvoice: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 24
    Top = 504
  end
  object ravDSRInvoice: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRInvoice
    Left = 32
    Top = 552
  end
  object qrDSRConsolidated: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 40
    Top = 600
  end
  object RAVDSRConsolidated: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRConsolidated
    Left = 40
    Top = 648
  end
  object qrDSRdeleteTable: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 112
    Top = 544
  end
  object qrDSRTrans: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 16
    Top = 720
  end
  object ravDSRMenuDay: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRMenuDay
    Left = 32
    Top = 256
  end
  object qrDSRMenuDay: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 48
    Top = 324
  end
  object sqlAddLK: TIBSQL
    Database = dmStockData.dbStock
    Transaction = dmStockData.trStock
    Left = 856
    Top = 352
  end
  object ravTaxProfile: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrTaxProfile
    Left = 944
    Top = 200
  end
  object qrTaxProfile: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 984
    Top = 200
  end
  object ravSalesSummaryB: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSalesSummaryB
    Left = 975
    Top = 280
  end
  object qrSalesSummaryB: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 1088
    Top = 280
  end
  object ravAccumulatedTotals: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrAccumulatedTotals
    Left = 1024
    Top = 320
  end
  object qrAccumulatedTotals: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 1088
    Top = 320
  end
  object ravAccumulatedZeds: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrAccumulatedZeds
    Left = 1024
    Top = 352
  end
  object qrAccumulatedZeds: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 1088
    Top = 360
  end
  object ravCancelTotal: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrCancelTotal
    Left = 1024
    Top = 389
  end
  object qrCancelTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 1088
    Top = 392
  end
  object ravPaymentTotal: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrPaymentTotal
    Left = 1024
    Top = 432
  end
  object qrPaymentTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 1088
    Top = 432
  end
  object ravDiscountTotal: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDiscountTotal
    Left = 1024
    Top = 472
  end
  object qrDiscountTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 1088
    Top = 476
  end
  object ravSalesRevenue: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSalesRevenue
    Left = 868
    Top = 306
  end
  object qrSalesRevenue: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 788
    Top = 430
  end
  object qrSalesConsolidated: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 932
    Top = 607
  end
  object ravSalesConsolidated: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSalesConsolidated
    Left = 862
    Top = 730
  end
  object ravSalesRevenueTotal: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesRevenueTotal
    Left = 858
    Top = 612
  end
  object qrSalesRevenueTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 917
    Top = 376
  end
  object ravSalesMenuSummary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSalesMenuSummary
    Left = 924
    Top = 520
  end
  object qrSalesMenuSummary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 896
    Top = 311
  end
  object qrDiscountTotalComplimentary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 1088
    Top = 240
  end
  object ravDiscountTotalComplimentary: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDiscountTotalComplimentary
    Left = 1024
    Top = 248
  end
  object ravCatDiscount: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrCatDiscount
    Left = 1160
    Top = 24
  end
  object qrDiscountTotalCorporate: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 960
    Top = 48
  end
  object ravDiscountTotalCorporate: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDiscountTotalCorporate
    Left = 1048
    Top = 48
  end
  object qrCatDiscount: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1272
    Top = 88
  end
  object qrSalesCountByDayPart: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 856
    Top = 820
  end
  object ravSalesCountByDayPart: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSalesCountByDayPart
    Left = 1064
    Top = 616
  end
  object qrDiscountTotalNoGroup: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 960
    Top = 672
  end
  object ravDiscountTotalNoGroup: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrDiscountTotalNoGroup
    Left = 1048
    Top = 672
  end
  object qrRefunds: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1533
    Top = 392
  end
  object ravRefunds: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrRefunds
    Left = 1402
    Top = 384
  end
  object qrCancelAdj: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 587
    Top = 152
  end
  object RvDataSetConnection1: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrAccumulatedTotals
    Left = 595
    Top = 192
  end
  object qrCancelsAdj: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1573
    Top = 272
  end
  object ravCancelsAdj: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrCancelsAdj
    Left = 1418
    Top = 264
  end
  object qrComplimentary: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 592
    Top = 304
  end
  object ravComplimentary: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrComplimentary
    Left = 584
    Top = 360
  end
  object qrDSRConsolidatedByItem: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 960
    Top = 769
  end
  object ravDSRConsolidatedByItem: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRConsolidatedByItem
    Left = 1237
    Top = 869
  end
  object ravSalesDiscountTotalLocation: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSalesDiscountTotalLocation
    Left = 1008
    Top = 120
  end
  object qrSalesDiscountTotalLocation: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 1080
    Top = 128
  end
  object ravCheckRemoval: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrCheckRemoval
    Left = 600
    Top = 432
  end
  object qrCheckRemoval: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 600
    Top = 480
  end
  object qrDSRCat: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 487
    Top = 620
  end
  object RvDataSetConnection2: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRConsolidatedByItem
    Left = 1237
    Top = 869
  end
  object qrDSRCategory: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1429
    Top = 480
  end
  object ravDSRCategory: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRCategory
    Left = 1530
    Top = 504
  end
  object qrDSRTax: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1421
    Top = 552
  end
  object ravDSRTax: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRTax
    Left = 1498
    Top = 568
  end
  object qrDSRDiscount: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1405
    Top = 624
  end
  object ravDSRDiscount: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRDiscount
    Left = 1514
    Top = 624
  end
  object qrDSRrefcan: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1381
    Top = 720
  end
  object ravDSRrefcan: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRrefcan
    Left = 1482
    Top = 728
  end
  object qrDSRSurcharge: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1389
    Top = 784
  end
  object ravDSRSurcharge: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRSurcharge
    Left = 1490
    Top = 797
  end
  object qrDSRPay: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1389
    Top = 843
  end
  object ravDSRPay: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRPay
    Left = 822
    Top = 521
  end
  object qrDSRSum: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1229
    Top = 805
  end
  object ravDSRSum: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDSRSum
    Left = 1290
    Top = 787
  end
  object qrReprintReceipt: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 600
    Top = 528
  end
  object ravReprintReceipt: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrReprintReceipt
    Left = 600
    Top = 600
  end
  object ravSalesConsolidatedCancel: TRvDataSetConnection
    RuntimeVisibility = rtEndUser
    DataSet = qrSalesConsolidatedCancel
    Left = 569
    Top = 400
  end
  object qrSalesConsolidatedCancel: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 649
    Top = 408
  end
  object qrResetPoints: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 900
    Top = 64
  end
  object ravResetPoint: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrResetPoints
    Left = 835
    Top = 64
  end
  object qrReprintOrder: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    Left = 592
    Top = 438
  end
  object ravReprintOrder: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrReprintOrder
    Left = 584
    Top = 548
  end
  object qrDiscountGrandTotal: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1345
    Top = 701
  end
  object ravDiscountGrandTotal: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrDiscountGrandTotal
    Left = 40
    Top = 453
  end
  object qrProfiltLoss: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1077
    Top = 824
  end
  object RavProfiltLoss: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrProfiltLoss
    Left = 1002
    Top = 830
  end
  object qrPointSpend: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    Left = 856
    Top = 820
  end
  object RvDataSetConnection3: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesJournal4
    Left = 520
    Top = 840
  end
  object RvDataSetConnection4: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesJournal4
    Left = 520
    Top = 840
  end
  object RvDataSetConnection5: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesJournal4
    Left = 520
    Top = 840
  end
  object ravPointSpend: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrPointSpend
    Left = 40
    Top = 648
  end
  object qrBreakdownCategory: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = MMTrans
    SQL.Strings = (
      '')
    Left = 1320
    Top = 176
  end
  object ravBreakdownCategory: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrBreakdownCategory
    Left = 1440
    Top = 152
  end
  object qrSalesSummaryD: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1229
    Top = 480
  end
  object ravSalesSummaryD: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSalesSummaryD
    Left = 1226
    Top = 544
  end
  object qrSSDParemeter: TIBQuery
    Database = dmMMData.dbMenuMate
    Transaction = dmMMData.trMenuMate
    SQL.Strings = (
      '')
    Left = 1301
    Top = 488
  end
  object ravSSDParameter: TRvDataSetConnection
    RuntimeVisibility = rtDeveloper
    DataSet = qrSSDParemeter
    Left = 1314
    Top = 544
  end
end
